#ifndef _GRID_H_
#define _GRID_H_

#include <iostream>
#include <cmath>
#include <windows.h>
#include <vector>
#include <algorithm>
#include <random>

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
        std::vector<int> IndexBombs;
        std::vector<int> IndexNails;
        std::vector<int> IndexDusts;
    public:
        Grid();
        Grid(int ratioCell,int numberLines,int numberColumns,int offsetXLeft,int offsetXRight,int offsetYTop,int offsetYBottom,COLORREF gridColor,std::vector<int> indexBombs,std::vector<int> indexNails,std::vector<int> indexDusts);

        // Méthodes de la classe
        void windowChanged(int widthWindow, int heightWindow);
        void init(int widthWindow, int heightWindow);
        int checkGrid(int rowIndex, int columnIndex) const;
        void fillGridWithElements(const std::vector<int>& indexToAvoid);

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
        const std::vector<int>& getIndexBombs() const;
        const std::vector<int>& getIndexNails() const;
        const std::vector<int>& getIndexDusts() const;

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
        void setIndexBombs(const std::vector<int>& indexBombs);
        void setIndexNails(const std::vector<int>& indexNails);
        void setIndexDusts(const std::vector<int>& indexDusts);
};
#endif