#ifndef _GRID_H_
#define _GRID_H_

#include <iostream>
#include <cmath>
#include <windows.h>

class Grid{
    private:
        int RatioCell;
        int CellWidth;
        int CellHeight;
        int NumberLines;
        int NumberColumns;
        int OffsetXLeft;
        int OffsetXRight;
        int OffsetYTop;
        int OffsetYBottom;
        COLORREF GridColor;
    public:
        Grid();
        Grid(int ratioCell,int numberLines,int numberColumns,int offsetXLeft,int offsetXRight,int offsetYTop,int offsetYBottom, COLORREF gridColor);

        // Méthodes de la classe
        void windowChanged(int widthWindow, int heightWindow);
        void init(int widthWindow, int heightWindow);

        // Getters
        int getRatioCell() const;
        int getNumberLines() const;
        int getNumberColumns() const;
        int getOffsetXLeft() const;
        int getOffsetXRight() const;
        int getOffsetYTop() const;
        int getOffsetYBottom() const;
        int getCellWidth() const;
        int getCellHeight() const;
        COLORREF getGridColor() const;

        // Setters
        void setRatioCell(int ratioCell);
        void setNumberLines(int numberLines);
        void setNumberColumns(int numberColumns);
        void setOffsetXLeft(int offsetXLeft);
        void setOffsetXRight(int offsetXRight);
        void setOffsetYTop(int offsetYTop);
        void setOffsetYBottom(int offsetYBottom);
        void setCellWidth(int cellWidth);
        void setCellHeight(int cellHeight);
        void setGridColor(COLORREF gridColor);
};
#endif