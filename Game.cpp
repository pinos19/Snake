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
}
void Game::drawGrid(const Grid& grid, HDC& hdc, const COLORREF colorGrid) {
    // Code pour dessiner la grille
}
void Game::drawSnake(const Snake& snake, HDC& hdc, const COLORREF colorSnake) {
    // Code pour dessiner le serpent
}
void Game::windowChanged(Snake& snake, Grid& grid, int widthWindow, int heightWindow, HDC& hdc, const COLORREF colorSnake, const COLORREF colorGrid) {
    // Code pour gérer les changements de fenêtre
}
void Game::updateSnake(const RECT& rectOld, const RECT& newRect, HDC& hdc, const COLORREF colorSnake) {
    // Code pour mettre à jour le serpent
}