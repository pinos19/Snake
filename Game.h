#ifndef _GAME_H_
#define _GAME_H_

#include <iostream>
#include "Grid.h"
#include "Snake.h"
#include <windows.h>
#include <vector>

class Game{
    private:
        Game() {};
        static bool Initialized;
        static int PaintFlag;
        static int Score;
        static bool Play;
        static bool GridSet;
    public:
        // Getters
        static bool getInitialized();
        static int getPaintFlag();
        static int getScore();
        static bool getPlay();
        static bool getGridSet();

        // Setters
        static void setInitialized(bool initialized);
        static void setPaintFlag(int paintFlag);
        static void setPlay(bool play);
        static void setScore(int score);
        static void setGridSet(bool gridSet);

        // Méthodes
        void init();
        void clearFigure(const RECT& rectWindow, HDC hdc, const COLORREF colorBackground);
        void drawGrid(const Grid& grid, HDC hdc, const COLORREF colorGrid);
        void drawSnake(const Snake& snake, HDC hdc, const COLORREF colorSnake);
        void windowChanged(Snake& snake, Grid& grid, const RECT& newWindow, HDC hdc, const COLORREF colorSnake, const COLORREF colorGrid, const COLORREF colorBackground);
        void updateSnake(const RECT& rectOld, const RECT& newRect, HDC hdc, const COLORREF colorSnake, const COLORREF colorBackground);
};
#endif