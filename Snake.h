#ifndef _SNAKE_H_
#define _SNAKE_H_

#include <iostream>
#include <vector>
#include <cmath>
#include "Grid.h"
#include <windows.h>

class Snake{
    private:
        int Speed;
        std::vector<int> IndexColumn;
        std::vector<int> IndexRow;
        std::vector<int> Directions;
        int Size;
        std::vector<RECT> SnakeRect;
        COLORREF SnakeColor;
        RECT SnakePreviousRect;
    public:
        Snake();
        Snake(int speed, std::vector<int> indexColumn, std::vector<int> indexRow, std::vector<int> directions, int size, std::vector<RECT> snakeRect, COLORREF snakeColor, RECT snakePreviousRect);
        int move(const Grid& grid);
        void grow();
        void shrink();
        void init(const Grid& grid);
        void gridChanged(const Grid& grid);
        void addDirection(int direction);
        bool popDirection();
        bool peekDirection(int &direction) const;

        // Getters
        int getSpeed() const;
        const std::vector<int>& getIndexColumn() const;
        const std::vector<int>& getIndexRow() const;
        const std::vector<int>& getDirections() const;
        int getSize() const;
        const std::vector<RECT>& getSnakeRect() const;
        COLORREF getSnakeColor() const;
        const RECT& getSnakePreviousRect() const;

        // Setters
        void setSpeed(int speed);
        void setIndexColumn(const std::vector<int>& indexColumn);
        void setIndexRow(const std::vector<int>& indexRow);
        void setSize(int size);
        void setSnakeRect(const std::vector<RECT>& snakeRect);
        void setSnakeColor(COLORREF snakeColor);
        void setSnakePreviousRect(const RECT& snakePreviousRect);
        void setDirections(const std::vector<int>& directions);
};
#endif