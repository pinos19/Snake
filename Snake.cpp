#include "Snake.h"


// Méthodes de la classe
void Snake::init(const Grid& grid){
    // Fonction qui réinitialise le serpent aux paramètres initiaux
    Speed = 75;
    IndexColumn.clear();
    IndexRow.clear();
    Directions.clear();
    Directions.push_back(Snake::Direction::None);
    IsAlive = true;
    IndexColumn.push_back(ceil(static_cast<double> (grid.getNumberColumns()/2)));
    IndexRow.push_back(ceil(static_cast<double> (grid.getNumberLines()/2)));
    Color = RGB(255,255,255);
    Size = 1;
}
void Snake::addDirection(Snake::Direction direction){
    // Add a direction to the queue of
    Directions.push_back(direction);
}
bool Snake::popDirection() {
    if( Directions.size() > 1 ){
        Directions.pop_front();
        return true;
    }else{
        return false;
    }
}
Snake::Direction Snake::getCurrentDirection() const {
    return Directions.front();
}
Grid::TileContent Snake::move(Grid& grid){
    // Function which moves the snake and return the content of the new tile 
    // Bomb, Dust, Empty, Nail or Snake
    // Return false if the snake is now dead

    int rowOld = IndexRow.front();
    int columnOld = IndexColumn.front();
    int newRow {rowOld}, newColumn {columnOld}, i, columnTemp, rowTemp;

    switch( Directions.front() ){
        case Snake::Direction::Up:{
            newRow -= 1;
            if( rowOld == 1 ){
                newRow = grid.getNumberLines();
            }
            break;
        }
        case Snake::Direction::Right:{
            newColumn += 1;
            if( columnOld == grid.getNumberColumns() ){
                newColumn = 1;
            }
            break;
        }
        case Snake::Direction::Down:{
            newRow += 1;
            if( rowOld == grid.getNumberLines() ){
                newRow = 1;
            }
            break;
        }
        case Snake::Direction::Left:{
            newColumn -= 1;
            if( columnOld == 1 ){
                newColumn = grid.getNumberColumns();
            }
            break;
        }
        default:{break;}
    }
    Grid::TileContent tileContent = grid.getContentTile(newRow, newColumn);

    // Rotate vectors from 1 to the right
    std::rotate(IndexColumn.rbegin(), IndexColumn.rbegin() + 1, IndexColumn.rend());
    std::rotate(IndexRow.rbegin(), IndexRow.rbegin() + 1, IndexRow.rend());

    // Affect new values for the head of the snake
    IndexColumn.at(0) = newColumn;
    IndexRow.at(0) = newRow;

    if( isSnake(newRow, newColumn) ){
        tileContent = Grid::TileContent::Snake;
        IsAlive = false;
    }else{
        switch(tileContent){
            case Grid::TileContent::Bomb:{
                IsAlive = false;
                break;
            }
            case Grid::TileContent::Nail:{
                shrink(1);
                break;
            }
            case Grid::TileContent::Dust:{
                grow(1);
                break;
            }
        }   
    }

    return tileContent;
}
void Snake::grow(int growingValue){
    // Function which allows us to grow the snake
    for(int i=0; i<growingValue; i++){
        IndexColumn.push_back(IndexColumn.back());
        IndexRow.push_back(IndexRow.back());
    }
    Size += growingValue;
}
bool Snake::shrink(int shrinkingValue){
    // Function which allows us to shrink the snake with a value
    // Return false, if the snake is dead
    if( shrinkingValue >= Size ){
        IsAlive = false;
        return false;
    }else{
        for(int i =0; i<shrinkingValue; i++){
            IndexColumn.pop_back();
            IndexRow.pop_back();
        }
        Size-=shrinkingValue;
        return true;
    }
}
bool Snake::isSnake(int rowIndex, int columnIndex) const{
    // Fonction qui permet de dire si la case ciblée cible une case
    // serpent ou non

    std::vector<int>::const_iterator it1 = std::find(IndexRow.begin(), IndexRow.end(), rowIndex);
    if( it1 != IndexRow.end() ){
        if( std::find(IndexColumn.begin(), IndexColumn.end(), columnIndex) != IndexColumn.end() ){
            return true;
        }
    }
    return false;
}
std::vector<std::pair<int, int>> Snake::immunitySnake(const Grid& grid, int immunityDistanceHead) const{
    // Function which draws an immunity rectangle around the snake in order to avoid
    // putting elements on the snake when the grid is refreshing
    std::vector<std::pair<int, int>> immunityIndex;
    int rowHead = IndexRow.front();
    int columnHead = IndexColumn.front();
    int left {1}, top {1}, bottom{grid.getNumberLines()}, right{grid.getNumberColumns()};

    // We immune the snake, the row and column indexes
    for(int i = 0; i < Size; i++){
        immunityIndex.emplace_back(IndexRow.at(i), IndexColumn.at(i));
    }

    // Now we draw a rectangle around the head of the snake. The rectangle is limited to the 
    // window dimensions. It means that the constraint of the immunityDistanceHead may not
    // be respected if the head is too close from the border
    if( rowHead - immunityDistanceHead + 1 >= 0){
        // We do not exceed top
        top = rowHead - immunityDistanceHead;
    }
    if( grid.getNumberLines() - rowHead >= immunityDistanceHead){
        // We do not exceed bottom
        bottom = rowHead + immunityDistanceHead;
    }
    if( columnHead - immunityDistanceHead + 1 >= 0){
        // We do not exceed left
        left = columnHead - immunityDistanceHead;
    }
    if( grid.getNumberColumns() - columnHead >= immunityDistanceHead){
        // We do not exceed right
        right = columnHead + immunityDistanceHead;
    }

    // Now we immune all the indexes in the rectangle of the head of the snake
    int numberColumns = right - left + 1;
    int numberRows = bottom - top + 1;

    for(int i = 0; i < numberRows; i++){
        for(int j = 0; j < numberColumns; j++){
            immunityIndex.emplace_back(top + i, left + j);
        }
    }
    return immunityIndex;
}

// void Snake::gridChanged(const Grid& grid){
//     // Fonction qui actualise le serpent lorsque la grille change
//     for(int i = 0; i <Size; i++){
//         SnakeRect.at(i) = {grid.getOffsetXLeft()+(IndexColumn.at(i)-1)*(grid.getCellWidth()+1)+2, grid.getOffsetYTop() + (IndexRow.at(i)-1)*(grid.getCellHeight()+1)+2, grid.getOffsetXLeft() + IndexColumn.at(i)*(grid.getCellWidth()+1)-1, grid.getOffsetYTop() + IndexRow.at(i)*(grid.getCellHeight()+1)-1};
//     }
// }

RECT Snake::invalidateSnake(const std::pair<int,int> &lastCellSnake, const Grid &grid) const{
    // Function which allows us to get the rectangular area of the current snake and the last
    // rectangle of the snake

    // Copy the vector to find the min and max
    std::vector<int> rowTemp {IndexRow};
    rowTemp.push_back(lastCellSnake.first);
    std::vector<int> colTemp {IndexColumn};
    rowTemp.push_back(lastCellSnake.second);

    // Max and Min of each, row and column
    std::pair<std::vector<int>::iterator, std::vector<int>::iterator> minMaxRow = std::minmax_element(rowTemp.begin(), rowTemp.end());
    std::pair<std::vector<int>::iterator, std::vector<int>::iterator> minMaxCol = std::minmax_element(rowTemp.begin(), rowTemp.end());

    int left = grid.getOffsetXLeft()+(*minMaxCol.first-1)*(grid.getCellWidth()+1);
    int top = grid.getOffsetYTop() + (*minMaxRow.first-1)*(grid.getCellHeight()+1);
    int right = grid.getOffsetXLeft() + *minMaxCol.second*(grid.getCellWidth()+1)+1;
    int bottom = grid.getOffsetYTop() + *minMaxRow.second*(grid.getCellHeight()+1)+1;
    
    return RECT {left, top, right, bottom};
}

// Getters
int Snake::getSpeed() const { return Speed;}
const std::vector<int>& Snake::getIndexColumn() const { return IndexColumn; }
const std::vector<int>& Snake::getIndexRow() const { return IndexRow; }
int Snake::getSize() const { return Size; }
COLORREF Snake::getColor() const{ return Color; }
const std::deque<Snake::Direction>& Snake::getDirections() const{ return Directions; }
bool Snake::getIsAlive() const { return IsAlive; }

// Setters
void Snake::setSpeed(int speed) { Speed = speed; }
void Snake::setIndexColumn(const std::vector<int>& indexColumn) { IndexColumn = indexColumn; }
void Snake::setIndexRow(const std::vector<int>& indexRow) { IndexRow = indexRow; }
void Snake::setSize(int size) { Size = size; }
void Snake::setColor(COLORREF color){ Color = color; }
void Snake::setDirections(const std::deque<Snake::Direction>& directions){ Directions = directions; }
void Snake::setIsAlive(bool isAlive){ IsAlive = isAlive; }