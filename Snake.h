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
        int CurrentDirection;
        int PreviousDirection;
        int Size;
        std::vector<RECT> SnakeRect;
    public:
        Snake();
        Snake(int speed, std::vector<int> indexColumn, std::vector<int> indexRow, int currentDirection, int previousDirection, int size, std::vector<RECT> snakeRect);
        void move(const Grid& grid);
        void grow();
        void shrink();
        void init(const Grid& grid);
        void gridChanged(const Grid& grid);

        // Getters
        int getSpeed() const;
        const std::vector<int>& getIndexColumn() const;
        const std::vector<int>& getIndexRow() const;
        int getCurrentDirection() const;
        int getPreviousDirection() const;
        int getSize() const;
        const std::vector<RECT>& getSnakeRect() const;

        // Setters
        void setSpeed(int speed);
        void setIndexColumn(const std::vector<int>& indexColumn);
        void setIndexRow(const std::vector<int>& indexRow);
        void setCurrentDirection(int currentDirection);
        void setPreviousDirection(int previousDirection);
        void setSize(int size);
        void setSnakeRect(const std::vector<RECT>& snakeRect);
};
#endif