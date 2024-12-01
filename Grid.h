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

        void windowChanged(int widthWindow, int heightWindow);

        // Getters
        double getRatioCell() const;
        double getNumberLines() const;
        double getNumberColumns() const;
        double getOffsetXLeft() const;
        double getOffsetXRight() const;
        double getOffsetYTop() const;
        double getOffsetYBottom() const;

        // Setters
        void setRatioCell(double ratioCell);
        void setNumberLines(double numberLines);
        void setNumberColumns(double numberColumns);
        void setOffsetXLeft(double offsetXLeft);
        void setOffsetXRight(double offsetXRight);
        void setOffsetYTop(double offsetYTop);
        void setOffsetYBottom(double offsetYBottom);
};
#endif