#ifndef _SNAKE_H_
#define _SNAKE_H_

#include <iostream>
#include <vector>
#include <cmath>
#include "Grid.h"
#include <windows.h>
#include "utils_functions.h"

class Snake{
    private:
        int Speed;
        std::vector<int> IndexColumn;
        std::vector<int> IndexRow;
        std::vector<int> Directions;
        int Size;
        std::vector<RECT> SnakeRect;
        COLORREF SnakeColor;
        std::vector<RECT> SnakePreviousRect;
    public:
        Snake();
        Snake(int speed, std::vector<int> indexColumn, std::vector<int> indexRow, std::vector<int> directions, int size, std::vector<RECT> snakeRect, COLORREF snakeColor, std::vector<RECT> snakePreviousRect);
        int move(Grid& grid);
        void grow();
        void shrink();
        void init(const Grid& grid);
        void gridChanged(const Grid& grid);
        void addDirection(int direction);
        bool popDirection();
        bool peekDirection(int &direction) const;
        bool isSnake(int rowIndex, int columnIndex);
        RECT invalidateSnake();
        std::vector<int> immunitySnake(const Grid& grid, int immunityDistanceHead) const;

        // Getters
        int getSpeed() const;
        const std::vector<int>& getIndexColumn() const;
        const std::vector<int>& getIndexRow() const;
        const std::vector<int>& getDirections() const;
        int getSize() const;
        const std::vector<RECT>& getSnakeRect() const;
        COLORREF getSnakeColor() const;
        const std::vector<RECT>& getSnakePreviousRect() const;

        // Setters
        void setSpeed(int speed);
        void setIndexColumn(const std::vector<int>& indexColumn);
        void setIndexRow(const std::vector<int>& indexRow);
        void setSize(int size);
        void setSnakeRect(const std::vector<RECT>& snakeRect);
        void setSnakeColor(COLORREF snakeColor);
        void setSnakePreviousRect(const std::vector<RECT>& snakePreviousRect);
        void setDirections(const std::vector<int>& directions);
};
#endif