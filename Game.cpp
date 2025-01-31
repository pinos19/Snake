#include "Game.h"

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
    // Drawing of the grid
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
void Game::drawSnake(HDC hdc) {
    // Function which draw the snake
    RECT rectTemp;

    HBRUSH hbrush = CreateSolidBrush(GameSnake.getColor());
    for(int i = 0; i <GameSnake.getSize(); i++){
        rectTemp = {GameGrid.getOffsetXLeft()+(GameSnake.getIndexColumn().at(i)-1)*(GameGrid.getCellWidth()+1)+2,
         GameGrid.getOffsetYTop() + (GameSnake.getIndexRow().at(i)-1)*(GameGrid.getCellHeight()+1)+2,
          GameGrid.getOffsetXLeft() + GameSnake.getIndexColumn().at(i)*(GameGrid.getCellWidth()+1)-1,
           GameGrid.getOffsetYTop() + GameSnake.getIndexRow().at(i)*(GameGrid.getCellHeight()+1)-1};
        FillRect(hdc, &rectTemp, hbrush);   
    }
    DeleteObject(hbrush);
}
void Game::drawElements(HDC hdc, const std::list<std::pair<int, int>> &elementList, COLORREF elementColor){
    // Fonction which allows us to draw element
    int iColumn {0}, iRow {0};

    // Color options
    HBRUSH hBrush = CreateSolidBrush(elementColor);
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
    HPEN hPen = CreatePen(PS_SOLID, 1, elementColor);
    HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);

    std::list<std::pair<int, int>>::const_iterator iteratorList {elementList.cbegin()};
    std::pair<int, int> pairTemp;

    while( iteratorList!=elementList.cend() ){
        pairTemp = *iteratorList;
        
        iRow = pairTemp.first;
        iColumn = pairTemp.second;

        Ellipse(hdc, GameGrid.getOffsetXLeft()+(iColumn-1)*(GameGrid.getCellWidth()+1)+2,
        GameGrid.getOffsetYTop() + (iRow-1)*(GameGrid.getCellHeight()+1)+2,
        GameGrid.getOffsetXLeft() + iColumn*(GameGrid.getCellWidth()+1)-1,
        GameGrid.getOffsetYTop() + iRow*(GameGrid.getCellHeight()+1)-1);

        iteratorList++;
    }

    // Restore ancient pen and brush
    SelectObject(hdc, hOldBrush);
    SelectObject(hdc, hOldPen);
    DeleteObject(hBrush);
    DeleteObject(hPen);
}


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