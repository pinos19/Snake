#include "Game.h"

// Initialisation des variables statiques
bool Game::Initialized = false;
int Game::PaintFlag = 0;
int Game::Score = 0;
bool Game::Play = false;
bool Game::GridSet = false;
COLORREF Game::BackgroundColor = RGB(0,0,0);

// Implémentation des getters et setters

// Getters
bool Game::getInitialized() {
    return Initialized;
}
int Game::getPaintFlag() {
    return PaintFlag;
}
int Game::getScore() {
    return Score;
}
bool Game::getPlay() {
    return Play;
}
bool Game::getGridSet() {
    return GridSet;
}
COLORREF Game::getBackgroundColor(){
    return BackgroundColor;
}

// Setters
void Game::setInitialized(bool initialized) {
    Initialized = initialized;
}
void Game::setPaintFlag(int paintFlag) {
    PaintFlag = paintFlag;
}
void Game::setScore(int score) {
    Score = score;
}
void Game::setPlay(bool play) {
    Play = play;
}
void Game::setGridSet(bool gridSet) {
    GridSet = gridSet;
}
void Game::setBackgroundColor(COLORREF backgroundColor){
    BackgroundColor = backgroundColor;
}


// Implémentation des méthodes
void Game::clearFigure(const RECT& rectWindow, HDC hdc, const COLORREF colorBackground){
    HBRUSH hbrush = CreateSolidBrush(colorBackground);
    FillRect(hdc, &rectWindow, hbrush);
    DeleteObject(hbrush);
}
void Game::init() {
    // Initialisation du jeu
    Initialized = false;
    PaintFlag = 0;
    Score = 0;
    Play = false;
    BackgroundColor = RGB(50,50,50);
}
void Game::drawGrid(const Grid& grid, HDC hdc, const COLORREF colorGrid) {
    // Code pour dessiner la grille
    int x, y;
    int lastColumn = grid.getCellWidth()*grid.getNumberColumns()+grid.getNumberColumns()+1+grid.getOffsetXLeft();
    int lastRow = grid.getCellHeight()*grid.getNumberLines()+grid.getNumberLines()+1+grid.getOffsetYTop();

    // Définir la brosse pour dessiner les lignes
    HPEN hPen = CreatePen(PS_SOLID, 1, colorGrid);
    HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);

    // Dessiner les lignes verticales
    for (x = grid.getOffsetXLeft(); x <= lastColumn; x += grid.getCellWidth()+1) {
        MoveToEx(hdc, x, grid.getOffsetYTop(), nullptr);
        LineTo(hdc, x, lastRow);
    }

    // Dessiner les lignes horizontales
    for (y = grid.getOffsetYTop(); y <= lastRow; y += grid.getCellHeight()+1) {
        MoveToEx(hdc, grid.getOffsetXLeft(), y, nullptr);
        LineTo(hdc, lastColumn, y);
    }

    // Restaurer la brosse précédente et nettoyer
    SelectObject(hdc, hOldPen);
    DeleteObject(hPen);
}
void Game::drawSnake(const Snake& snake, HDC hdc, const COLORREF colorSnake) {
    // Code pour dessiner le serpent

    HBRUSH hbrush = CreateSolidBrush(colorSnake);
    std::vector<RECT> snakeRectangle = snake.getSnakeRect();
    for(int i = 0; i <snake.getSize(); i++){
        FillRect(hdc, &snakeRectangle.at(i), hbrush);
    }
    DeleteObject(hbrush);
}
void Game::windowChanged(Snake& snake, Grid& grid, const RECT& newWindow, HDC hdc, const COLORREF colorSnake, const COLORREF colorGrid, const COLORREF colorBackground) {
    // Code pour gérer les changements de fenêtre

    // Nettoyage de la figure
    clearFigure(newWindow, hdc, colorBackground);

    // Actualisation de la grille et affichage
    int widthWindow = newWindow.right - newWindow.left;
    int heightWindow = newWindow.bottom - newWindow.top;
    grid.windowChanged(widthWindow, heightWindow);
    drawGrid(grid, hdc, colorGrid);

    // Actualisation serpent et affichage
    snake.gridChanged(grid);
    drawSnake(snake, hdc, colorSnake);

    // Actualisation élements
    drawElements(grid, hdc, RGB(255,0,0), RGB(255,255,0), RGB(0,255,0));
}
void Game::updateSnake(Snake& snake, HDC hdc, const COLORREF colorSnake, const COLORREF colorBackground) {
    // Code pour mettre à jour le serpent, uniquement après un déplacement

    HBRUSH hbrushBack = CreateSolidBrush(colorBackground);
    HBRUSH hbrushSnake = CreateSolidBrush(colorSnake);

    const std::vector<RECT>& snakePrevious = snake.getSnakePreviousRect();
    const RECT& snakeLastRect = snake.getSnakeRect().back();

    for(int i = 0; i< snakePrevious.size(); i++){
        if( !EqualRect(&snakePrevious.at(i), &snakeLastRect) ){
            FillRect(hdc, &snakePrevious.at(i), hbrushBack);
        }
    }
    FillRect(hdc, &snake.getSnakeRect().front(), hbrushSnake);

    DeleteObject(hbrushBack);
    DeleteObject(hbrushSnake);
}
void Game::actualizeGridElements(Grid& grid, const Snake& snake){
    // Fonction qui permet d'actualiser la position des bombes, de la poussière et des clous en fonction
    // de la position du serpent

    // On récupère les index sur lesquels ne pas placer de bombes, de clous ou de poussières
    std::vector<int> indexImmunitySnake = snake.immunitySnake(grid, 5);

    // On réinitialise les éléments sur la grille
    grid.fillGridWithElements(indexImmunitySnake);
}
void Game::drawElements(const Grid& grid, HDC hdc, const COLORREF colorBomb, const COLORREF colorNail, const COLORREF colorDust){
    // Fonction qui permet de dessiner sur la figure les éléments de la grille

    drawBomb(grid, hdc, colorBomb);
    drawNail(grid, hdc, colorNail);
    drawDust(grid, hdc, colorDust);
}
void Game::drawBomb(const Grid& grid, HDC hdc, const COLORREF colorBomb){
    int i {0}, index {0}, iColumn {0}, iRow {0};
    const std::vector<int>& indexBombs = grid.getIndexBombs();

    HBRUSH hBrush = CreateSolidBrush(colorBomb);
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
    HPEN hPen = CreatePen(PS_SOLID, 1, colorBomb);
    HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);

    // On boucle sur chaque vecteur et on dessine les éléments
    for(i = 0; i< indexBombs.size(); i++){
        // Dessine un cercle pour la bombe
        index = indexBombs.at(i);
        iColumn = std::ceil(static_cast<double> (index)/grid.getNumberLines());
        iRow = grid.getNumberLines() - (iColumn*grid.getNumberLines() - index);
        Ellipse(hdc, grid.getOffsetXLeft()+(iColumn-1)*(grid.getCellWidth()+1)+2,
        grid.getOffsetYTop() + (iRow-1)*(grid.getCellHeight()+1)+2,
        grid.getOffsetXLeft() + iColumn*(grid.getCellWidth()+1)-1,
        grid.getOffsetYTop() + iRow*(grid.getCellHeight()+1)-1);
    }
    // Restauration des anciens pinceaux et stylos
    SelectObject(hdc, hOldBrush);
    SelectObject(hdc, hOldPen);
    DeleteObject(hBrush);
    DeleteObject(hPen);
}
void Game::drawNail(const Grid& grid, HDC hdc, const COLORREF colorNail){
    int i {0}, index {0}, iColumn {0}, iRow {0};
    const std::vector<int>& indexNails = grid.getIndexNails();

    HBRUSH hBrush = CreateSolidBrush(colorNail);
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
    HPEN hPen = CreatePen(PS_SOLID, 1, colorNail);
    HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
    for(i = 0; i< indexNails.size(); i++){
        // Dessine un cercle pour le clou 
        index = indexNails.at(i);
        iColumn = std::ceil(static_cast<double> (index)/grid.getNumberLines());
        iRow = grid.getNumberLines() - (iColumn*grid.getNumberLines() - index);
        Ellipse(hdc, grid.getOffsetXLeft()+(iColumn-1)*(grid.getCellWidth()+1)+2,
        grid.getOffsetYTop() + (iRow-1)*(grid.getCellHeight()+1)+2,
        grid.getOffsetXLeft() + iColumn*(grid.getCellWidth()+1)-1,
        grid.getOffsetYTop() + iRow*(grid.getCellHeight()+1)-1);
    }
    // Restauration des anciens pinceaux et stylos
    SelectObject(hdc, hOldBrush);
    SelectObject(hdc, hOldPen);
    DeleteObject(hBrush);
    DeleteObject(hPen);
}
void Game::drawDust(const Grid& grid, HDC hdc, const COLORREF colorDust){
    int i {0}, index {0}, iColumn {0}, iRow {0};
    const std::vector<int>& indexDusts = grid.getIndexDusts();

    HBRUSH hBrush = CreateSolidBrush(colorDust);
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
    HPEN hPen = CreatePen(PS_SOLID, 1, colorDust);
    HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
    for(i = 0; i< indexDusts.size(); i++){
        // Dessine un cercle pour la poussière 
        index = indexDusts.at(i);
        iColumn = std::ceil(static_cast<double> (index)/grid.getNumberLines());
        iRow = grid.getNumberLines() - (iColumn*grid.getNumberLines() - index);
        Ellipse(hdc, grid.getOffsetXLeft()+(iColumn-1)*(grid.getCellWidth()+1)+2,
        grid.getOffsetYTop() + (iRow-1)*(grid.getCellHeight()+1)+2,
        grid.getOffsetXLeft() + iColumn*(grid.getCellWidth()+1)-1,
        grid.getOffsetYTop() + iRow*(grid.getCellHeight()+1)-1);
    }
    // Restauration des anciens pinceaux et stylos
    SelectObject(hdc, hOldBrush);
    SelectObject(hdc, hOldPen);
    DeleteObject(hBrush);
    DeleteObject(hPen);
}