#ifndef _GAME_H_
#define _GAME_H_

#include "Grid.h"
#include "Snake.h"

class Game{
    public:
        enum class StateGame {Playing, Pause, Dead, Menu};
        enum class PaintFlag {InitApp, InitGrid, Moving, WinResized, ResumeGame};

        Grid GameGrid;
        Snake GameSnake;
        COLORREF BackgroundColor;
        bool Initialized;
        int Score;
        StateGame GameState;
        PaintFlag GamePaintFlag;

        Game() = default;

        // Methods
        void init(int widthWindow, int heightWindow);
        void clearFigure(const RECT& rectWindow, HDC hdc);
        void drawGrid(HDC hdc);
        void drawSnake(HDC hdc);
        void windowChanged(const RECT& newWindow, HDC hdc);
        void updateSnake(HDC hdc);
        void actualizeGridElements();
        void drawElements(HDC hdc, const std::list<std::pair<int, int>> &elementList, COLORREF elementColor);
        void drawBomb(HDC hdc);
        void drawNail(HDC hdc);
        void drawDust(HDC hdc);
        RECT snakeFormFromIndex(int indexRow, int indexCol);
};

#endif