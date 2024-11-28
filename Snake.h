#ifndef _SNAKE_H_
#define _SNAKE_H_

#include <iostream>
#include <vector>

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
};





#endif