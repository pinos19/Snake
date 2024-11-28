#ifndef _GRID_H_
#define _GRID_H_

#include <iostream>

class Grid{
    private:
        double RatioCell;
        double NumberLines;
        double NumberColumns;
        double OffsetXLeft;
        double OffsetXRight;
        double OffsetYTop;
        double OffsetYBottom;
    public:
        Grid();
        Grid(double ratioCell,double numberLines,double numberColumns,double offsetXLeft,double offsetXRight,double offsetYTop,double offsetYBottom);
};





#endif