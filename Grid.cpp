#include "Grid.h"


// Constructeurs
Grid::Grid()
    :RatioCell{},NumberLines{},NumberColumns{},OffsetXLeft{},OffsetXRight{},OffsetYTop{},OffsetYBottom{}{}
Grid::Grid(double ratioCell,double numberLines,double numberColumns,double offsetXLeft,double offsetXRight,double offsetYTop,double offsetYBottom)
    :RatioCell{ratioCell},NumberLines{numberLines},NumberColumns{numberColumns},OffsetXLeft{offsetXLeft},OffsetXRight{offsetXRight},OffsetYTop{offsetYTop},OffsetYBottom{offsetYBottom}{}

// Méthodes de la classe
void Grid::windowChanged(int widthWindow, int heightWindow){

}

// Getters
double Grid::getRatioCell() const {
    return RatioCell;
}
double Grid::getNumberLines() const {
    return NumberLines;
}
double Grid::getNumberColumns() const {
    return NumberColumns;
}
double Grid::getOffsetXLeft() const {
    return OffsetXLeft;
}
double Grid::getOffsetXRight() const {
    return OffsetXRight;
}
double Grid::getOffsetYTop() const {
    return OffsetYTop;
}
double Grid::getOffsetYBottom() const {
    return OffsetYBottom;
}

// Setters
void Grid::setRatioCell(double ratioCell){
    RatioCell = ratioCell;
}
void Grid::setNumberLines(double numberLines) {
    NumberLines = numberLines;
}
void Grid::setNumberColumns(double numberColumns) {
    NumberColumns = numberColumns;
}
void Grid::setOffsetXLeft(double offsetXLeft) {
    OffsetXLeft = offsetXLeft;
}
void Grid::setOffsetXRight(double offsetXRight) {
    OffsetXRight = offsetXRight;
}
void Grid::setOffsetYTop(double offsetYTop) {
    OffsetYTop = offsetYTop;
}
void Grid::setOffsetYBottom(double offsetYBottom) {
    OffsetYBottom = offsetYBottom;
}