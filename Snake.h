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
        void move(Grid grid);
        void grow();
        void shrink();
        void init();
};





#endif