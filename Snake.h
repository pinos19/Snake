#ifndef _SNAKE_H_
#define _SNAKE_H_

#include <iostream>
#include <cmath>
#include <windows.h>
#include <vector>
#include <deque>
#include <algorithm>
#include <utility>
#include "Grid.h"

class Snake{
    public:
        enum class Direction {Right, Left, Up, Down, None};
    private:
        int Speed;
        int Size;
        std::vector<int> IndexColumn;
        std::vector<int> IndexRow;
        std::vector<std::pair<int,int>> cellTail;
        std::deque<Direction> Directions;
        bool IsAlive;
        COLORREF Color;
    public:
        // Constructor
        Snake() = default;
        void init(const Grid& grid);
        void addDirection(Direction direction);
        bool popDirection();
        Direction getCurrentDirection() const;
        Grid::TileContent move(Grid& grid);
        void grow(int growingValue);
        bool shrink(int shrinkingValue);
        std::vector<std::pair<int, int>> immunitySnake(const Grid& grid, int immunityDistanceHead) const;
        bool isSnake(int rowIndex, int columnIndex) const;
        RECT invalidateSnake(const Grid &grid) const;

        // Getters
        int getSpeed() const;
        const std::vector<int>& getIndexColumn() const;
        const std::vector<int>& getIndexRow() const;
        const std::deque<Direction>& getDirections() const;
        int getSize() const;
        COLORREF getColor() const;
        bool getIsAlive() const;
        const std::vector<std::pair<int,int>> getCellTail() const; 

        // Setters
        void setSpeed(int speed);
        void setIndexColumn(const std::vector<int>& indexColumn);
        void setIndexRow(const std::vector<int>& indexRow);
        void setSize(int size);
        void setColor(COLORREF color);
        void setDirections(const std::deque<Direction>& directions);
        void setIsAlive(bool isAlive);
};
#endif