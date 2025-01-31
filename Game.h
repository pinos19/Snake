#ifndef _GAME_H_
#define _GAME_H_

#include "Grid.h"
#include "Snake.h"

class Game{
    public:
        enum class StateGame {Playing, Pause, Dead, Menu};
        enum class PaintFlag {InitApp, InitGrid, Moving, WinResized, ContinueGame};
    private:
        Grid GameGrid;
        Snake GameSnake;
        COLORREF BackgroundColor;
        bool Initialized;
        int Score;
        StateGame GameState;
        PaintFlag GamePaintFlag;
    public:
        Game() = default;

        // Getters
        const Grid &getGameGrid() const;
        const Snake &getGameSnake() const;
        COLORREF getBackgroundColor() const;
        bool getInitialized() const;
        int getScore() const;
        StateGame getGameState() const;
        PaintFlag getGamePaintFlag() const;

        // Setters
        void setGameGrid(const Grid &grid);
        void setGameSnake(const Snake &snake);
        void setBackgroundColor(COLORREF backgroundColor);
        void setInitialized(bool initialized);
        void setScore(int score);
        void setGameState(StateGame gameState);
        void setGamePaintFlag(PaintFlag gamePaintFlag);

        // Méthodes
        void init(int widthWindow, int heightWindow);
        void clearFigure(const RECT& rectWindow, HDC hdc);
        void drawGrid(HDC hdc);
        void drawSnake(HDC hdc);
        void windowChanged(const RECT& newWindow, HDC hdc);
        void updateSnake(HDC hdc);
        void actualizeGridElements();
        void drawElements(HDC hdc);
        void drawBomb(HDC hdc);
        void drawNail(HDC hdc);
        void drawDust(HDC hdc);
};

#endif