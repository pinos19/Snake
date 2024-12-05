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
}
void Game::updateSnake(Snake& snake, HDC hdc, const COLORREF colorSnake, const COLORREF colorBackground) {
    // Code pour mettre à jour le serpent, uniquement après un déplacement

    HBRUSH hbrushBack = CreateSolidBrush(colorBackground);
    HBRUSH hbrushSnake = CreateSolidBrush(colorSnake);

    if( !EqualRect(&snake.getSnakePreviousRect(), &snake.getSnakeRect().at(snake.getSize()-1)) ){
        FillRect(hdc, &snake.getSnakePreviousRect(), hbrushBack);
    }
    FillRect(hdc, &snake.getSnakeRect().front(), hbrushSnake);

    DeleteObject(hbrushBack);
    DeleteObject(hbrushSnake);
}