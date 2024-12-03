#include "Game.h"

// Initialisation des variables statiques
bool Game::Initialized = false;
int Game::PaintFlag = 0;
int Game::Score = 0;
bool Game::Play = false;

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

// Implémentation des méthodes
void Game::init() {
    // Initialisation du jeu
    Initialized = false;
    PaintFlag = 0;
    Score = 0;
    Play = false;
}
void Game::drawGrid(const Grid& grid, HDC& hdc, const COLORREF colorGrid) {
    // Code pour dessiner la grille
    int x, y;
    int lastColumn = grid.getCellWidth()*grid.getNumberColumns()+grid.getNumberColumns()+1+grid.getOffsetXLeft();
    int lastRow = grid.getCellHeight()*grid.getNumberLines()+grid.getNumberLines()+1+grid.getOffsetYTop();

    // Définir la brosse pour dessiner les lignes
    HPEN hPen = CreatePen(PS_SOLID, 1, colorGrid);
    HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);

    // Dessiner les lignes verticales
    for (x = grid.getOffsetXLeft() + 1; x <= lastColumn; x += grid.getCellWidth()+1) {
        MoveToEx(hdc, x, grid.getOffsetYTop()+1, nullptr);
        LineTo(hdc, x, lastRow);
    }

    // Dessiner les lignes horizontales
    for (y = grid.getOffsetYTop() + 1; y <= lastRow; y += grid.getCellHeight()+1) {
        MoveToEx(hdc, grid.getOffsetXLeft()+1, y, nullptr);
        LineTo(hdc, lastColumn, y);
    }

    // Restaurer la brosse précédente et nettoyer
    SelectObject(hdc, hOldPen);
    DeleteObject(hPen);
}
void Game::drawSnake(const Snake& snake, HDC& hdc, const COLORREF colorSnake) {
    // Code pour dessiner le serpent

    HBRUSH hbrush = CreateSolidBrush(colorSnake);
    std::vector<RECT> snakeRectangle = snake.getSnakeRect();
    for(int i = 0; i < snake.getSize()-1; i++){
        FillRect(hdc, &snakeRectangle.at(i), hbrush);
    }
    DeleteObject(hbrush);
}
void Game::windowChanged(Snake& snake, Grid& grid, int widthWindow, int heightWindow, HDC& hdc, const COLORREF colorSnake, const COLORREF colorGrid) {
    // Code pour gérer les changements de fenêtre
}
void Game::updateSnake(const RECT& rectOld, const RECT& newRect, HDC& hdc, const COLORREF colorSnake) {
    // Code pour mettre à jour le serpent
}