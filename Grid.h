#ifndef _GRID_H_
#define _GRID_H_

#include <iostream>
#include <cmath>
#include <windows.h>
#include <vector>
#include <algorithm>
#include <random>
#include <utility>
#include "Snake.h"
#include <list>

class Grid{
    public:
        enum class TileContent {Bomb, Nail, Dust, Snake, Empty};
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
        std::list<std::pair<int, int>> IndexBombs;
        std::list<std::pair<int, int>> IndexNails;
        std::list<std::pair<int, int>> IndexDusts;
    public:
        Grid() = default;

        // Methods
        void init(int widthWindow, int heightWindow);
        void windowChanged(int widthWindow, int heightWindow);
        void fillGridWithElements(const std::vector<std::pair<int, int>>& indexesToAvoid);
        bool popGrid(int rowIndex, int columnIndex);
        TileContent getContentTile(int rowIndex, int ColumnIndex, const Snake &snake);
        TileContent getContentTile(int rowIndex, int ColumnIndex, std::list<std::pair<int, int>>::iterator &it);

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
        const std::vector<std::pair<int, int>>& getIndexBombs() const;
        const std::vector<std::pair<int, int>>& getIndexNails() const;
        const std::vector<std::pair<int, int>>& getIndexDusts() const;

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
        void setIndexBombs(const std::vector<std::pair<int, int>>& indexBombs);
        void setIndexNails(const std::vector<std::pair<int, int>>& indexNails);
        void setIndexDusts(const std::vector<std::pair<int, int>>& indexDusts);
};
#endif