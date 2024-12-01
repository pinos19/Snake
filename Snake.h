#ifndef _SNAKE_H_
#define _SNAKE_H_

#include <iostream>
#include <vector>
#include "Grid.h"

class Snake{
    private:
        double Speed;
        std::vector<int> IndexColumn;
        std::vector<int> IndexRow;
        int CurrentDirection;
        int PreviousDirection;
        int Size;
    public:
        Snake();
        Snake(double speed, std::vector<int> indexColumn, std::vector<int> indexRow, int currentDirection, int previousDirection, int size);
        void move(const Grid& grid);
        void grow();
        void shrink();
        void init(const Grid& grid);

        // Getters
        double getSpeed() const;
        const std::vector<int>& getIndexColumn() const;
        const std::vector<int>& getIndexRow() const;
        int getCurrentDirection() const;
        int getPreviousDirection() const;
        int getSize() const;

        // Setters
        void setSpeed(double speed);
        void setIndexColumn(const std::vector<int>& indexColumn);
        void setIndexRow(const std::vector<int>& indexRow);
        void setCurrentDirection(int currentDirection);
        void setPreviousDirection(int previousDirection);
        void setSize(int size);
};
#endif