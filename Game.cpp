#include "Game.h"

// Getters
const Grid &Game::getGameGrid() const{return GameGrid;}
const Snake &Game::getGameSnake() const{return GameSnake;}
COLORREF Game::getBackgroundColor() const{return BackgroundColor;}
bool Game::getInitialized() const{return Initialized;}
int Game::getScore() const{return Score;}
Game::StateGame Game::getGameState() const{return GameState;}
Game::PaintFlag Game::getGamePaintFlag() const{return GamePaintFlag;}

// Setters
void Game::setGameGrid(const Grid &grid){GameGrid = grid;}
void Game::setGameSnake(const Snake &snake){GameSnake = snake;}
void Game::setBackgroundColor(COLORREF backgroundColor){BackgroundColor = backgroundColor;}
void Game::setInitialized(bool initialized){Initialized = initialized;}
void Game::setScore(int score){Score = score;}
void Game::setGameState(StateGame gameState){GameState = gameState;}
void Game::setGamePaintFlag(PaintFlag gamePaintFlag){GamePaintFlag = gamePaintFlag;}

// Implémentation des méthodes
void Game::init(int widthWindow, int heightWindow) {
    // Initialisation du jeu
    GameGrid = Grid();
    GameGrid.init(widthWindow, heightWindow);
    GameSnake = Snake();
    GameSnake.init(GameGrid);
    BackgroundColor = RGB(50,50,50);
    Initialized = false;
    Score = 0;
    GameState = StateGame::Menu;
    GamePaintFlag = PaintFlag::InitApp;
}
void Game::clearFigure(const RECT& rectWindow, HDC hdc){
    HBRUSH hbrush = CreateSolidBrush(BackgroundColor);
    FillRect(hdc, &rectWindow, hbrush);
    DeleteObject(hbrush);
}
void Game::drawGrid(HDC hdc) {
    // Code pour dessiner la grille
    int x, y;
    int lastColumn = GameGrid.getCellWidth()*GameGrid.getNumberColumns()+GameGrid.getNumberColumns()+1+GameGrid.getOffsetXLeft();
    int lastRow = GameGrid.getCellHeight()*GameGrid.getNumberLines()+GameGrid.getNumberLines()+1+GameGrid.getOffsetYTop();

    // Définir la brosse pour dessiner les lignes
    HPEN hPen = CreatePen(PS_SOLID, 1, GameGrid.getGridColor());
    HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);

    // Dessiner les lignes verticales
    for (x = GameGrid.getOffsetXLeft(); x <= lastColumn; x += GameGrid.getCellWidth()+1) {
        MoveToEx(hdc, x, GameGrid.getOffsetYTop(), nullptr);
        LineTo(hdc, x, lastRow);
    }

    // Dessiner les lignes horizontales
    for (y = GameGrid.getOffsetYTop(); y <= lastRow; y += GameGrid.getCellHeight()+1) {
        MoveToEx(hdc, GameGrid.getOffsetXLeft(), y, nullptr);
        LineTo(hdc, lastColumn, y);
    }

    // Restaurer la brosse précédente et nettoyer
    SelectObject(hdc, hOldPen);
    DeleteObject(hPen);
}

// void Game::drawSnake(const Snake& snake, HDC hdc, const COLORREF colorSnake) {
//     // Code pour dessiner le serpent

//     HBRUSH hbrush = CreateSolidBrush(colorSnake);
//     std::vector<RECT> snakeRectangle = snake.getSnakeRect();
//     for(int i = 0; i <snake.getSize(); i++){
//         FillRect(hdc, &snakeRectangle.at(i), hbrush);
//     }
//     DeleteObject(hbrush);
// }
// void Game::windowChanged(Snake& snake, Grid& grid, const RECT& newWindow, HDC hdc, const COLORREF colorSnake, const COLORREF colorGrid, const COLORREF colorBackground) {
//     // Code pour gérer les changements de fenêtre

//     // Nettoyage de la figure
//     clearFigure(newWindow, hdc, colorBackground);

//     // Actualisation de la grille et affichage
//     int widthWindow = newWindow.right - newWindow.left;
//     int heightWindow = newWindow.bottom - newWindow.top;
//     grid.windowChanged(widthWindow, heightWindow);
//     drawGrid(grid, hdc, colorGrid);

//     // Actualisation serpent et affichage
//     snake.gridChanged(grid);
//     drawSnake(snake, hdc, colorSnake);

//     // Actualisation élements
//     drawElements(grid, hdc, RGB(255,0,0), RGB(255,255,0), RGB(0,255,0));
// }
// void Game::updateSnake(Snake& snake, HDC hdc, const COLORREF colorSnake, const COLORREF colorBackground) {
//     // Code pour mettre à jour le serpent, uniquement après un déplacement

//     HBRUSH hbrushBack = CreateSolidBrush(colorBackground);
//     HBRUSH hbrushSnake = CreateSolidBrush(colorSnake);

//     const std::vector<RECT>& snakePrevious = snake.getSnakePreviousRect();
//     const RECT& snakeLastRect = snake.getSnakeRect().back();

//     for(int i = 0; i< snakePrevious.size(); i++){
//         if( !EqualRect(&snakePrevious.at(i), &snakeLastRect) ){
//             FillRect(hdc, &snakePrevious.at(i), hbrushBack);
//         }
//     }
//     FillRect(hdc, &snake.getSnakeRect().front(), hbrushSnake);

//     DeleteObject(hbrushBack);
//     DeleteObject(hbrushSnake);
// }
// void Game::actualizeGridElements(Grid& grid, const Snake& snake){
//     // Fonction qui permet d'actualiser la position des bombes, de la poussière et des clous en fonction
//     // de la position du serpent

//     // On récupère les index sur lesquels ne pas placer de bombes, de clous ou de poussières
//     std::vector<int> indexImmunitySnake = snake.immunitySnake(grid, 5);

//     // On réinitialise les éléments sur la grille
//     grid.fillGridWithElements(indexImmunitySnake);
// }
// void Game::drawElements(const Grid& grid, HDC hdc, const COLORREF colorBomb, const COLORREF colorNail, const COLORREF colorDust){
//     // Fonction qui permet de dessiner sur la figure les éléments de la grille

//     drawBomb(grid, hdc, colorBomb);
//     drawNail(grid, hdc, colorNail);
//     drawDust(grid, hdc, colorDust);
// }
// void Game::drawBomb(const Grid& grid, HDC hdc, const COLORREF colorBomb){
//     int i {0}, index {0}, iColumn {0}, iRow {0};
//     const std::vector<int>& indexBombs = grid.getIndexBombs();

//     HBRUSH hBrush = CreateSolidBrush(colorBomb);
//     HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
//     HPEN hPen = CreatePen(PS_SOLID, 1, colorBomb);
//     HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);

//     // On boucle sur chaque vecteur et on dessine les éléments
//     for(i = 0; i< indexBombs.size(); i++){
//         // Dessine un cercle pour la bombe
//         index = indexBombs.at(i);
//         iColumn = std::ceil(static_cast<double> (index)/grid.getNumberLines());
//         iRow = grid.getNumberLines() - (iColumn*grid.getNumberLines() - index);
//         Ellipse(hdc, grid.getOffsetXLeft()+(iColumn-1)*(grid.getCellWidth()+1)+2,
//         grid.getOffsetYTop() + (iRow-1)*(grid.getCellHeight()+1)+2,
//         grid.getOffsetXLeft() + iColumn*(grid.getCellWidth()+1)-1,
//         grid.getOffsetYTop() + iRow*(grid.getCellHeight()+1)-1);
//     }
//     // Restauration des anciens pinceaux et stylos
//     SelectObject(hdc, hOldBrush);
//     SelectObject(hdc, hOldPen);
//     DeleteObject(hBrush);
//     DeleteObject(hPen);
// }
// void Game::drawNail(const Grid& grid, HDC hdc, const COLORREF colorNail){
//     int i {0}, index {0}, iColumn {0}, iRow {0};
//     const std::vector<int>& indexNails = grid.getIndexNails();

//     HBRUSH hBrush = CreateSolidBrush(colorNail);
//     HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
//     HPEN hPen = CreatePen(PS_SOLID, 1, colorNail);
//     HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
//     for(i = 0; i< indexNails.size(); i++){
//         // Dessine un cercle pour le clou 
//         index = indexNails.at(i);
//         iColumn = std::ceil(static_cast<double> (index)/grid.getNumberLines());
//         iRow = grid.getNumberLines() - (iColumn*grid.getNumberLines() - index);
//         Ellipse(hdc, grid.getOffsetXLeft()+(iColumn-1)*(grid.getCellWidth()+1)+2,
//         grid.getOffsetYTop() + (iRow-1)*(grid.getCellHeight()+1)+2,
//         grid.getOffsetXLeft() + iColumn*(grid.getCellWidth()+1)-1,
//         grid.getOffsetYTop() + iRow*(grid.getCellHeight()+1)-1);
//     }
//     // Restauration des anciens pinceaux et stylos
//     SelectObject(hdc, hOldBrush);
//     SelectObject(hdc, hOldPen);
//     DeleteObject(hBrush);
//     DeleteObject(hPen);
// }
// void Game::drawDust(const Grid& grid, HDC hdc, const COLORREF colorDust){
//     int i {0}, index {0}, iColumn {0}, iRow {0};
//     const std::vector<int>& indexDusts = grid.getIndexDusts();

//     HBRUSH hBrush = CreateSolidBrush(colorDust);
//     HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
//     HPEN hPen = CreatePen(PS_SOLID, 1, colorDust);
//     HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
//     for(i = 0; i< indexDusts.size(); i++){
//         // Dessine un cercle pour la poussière 
//         index = indexDusts.at(i);
//         iColumn = std::ceil(static_cast<double> (index)/grid.getNumberLines());
//         iRow = grid.getNumberLines() - (iColumn*grid.getNumberLines() - index);
//         Ellipse(hdc, grid.getOffsetXLeft()+(iColumn-1)*(grid.getCellWidth()+1)+2,
//         grid.getOffsetYTop() + (iRow-1)*(grid.getCellHeight()+1)+2,
//         grid.getOffsetXLeft() + iColumn*(grid.getCellWidth()+1)-1,
//         grid.getOffsetYTop() + iRow*(grid.getCellHeight()+1)-1);
//     }
//     // Restauration des anciens pinceaux et stylos
//     SelectObject(hdc, hOldBrush);
//     SelectObject(hdc, hOldPen);
//     DeleteObject(hBrush);
//     DeleteObject(hPen);
// }