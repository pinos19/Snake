#include "Snake.h"


// Constructeurs
Snake::Snake()
    :Speed{0},IndexColumn{},IndexRow{},CurrentDirection{0},PreviousDirection{0},Size{0},SnakeRect{}{}
Snake::Snake(int speed, std::vector<int> indexColumn, std::vector<int> indexRow, int currentDirection, int previousDirection, int size, std::vector<RECT> snakeRect)
    :Speed{speed}, IndexColumn{indexColumn}, IndexRow{indexRow}, CurrentDirection{currentDirection}, PreviousDirection{previousDirection}, Size{size}, SnakeRect{snakeRect}{}

// Méthodes de la classe
void Snake::move(const Grid& grid){
    // Fonction qui permet de déplacer le serpent avec la valeur de déplacement actuelle
    int columnOld = IndexColumn.at(0);
    int rowOld = IndexRow.at(0);
    int columnTemp, rowTemp, i;
    RECT rectOld = SnakeRect.at(0);
    RECT rectTemp;

    // On actualise la tête du serpent
    switch(CurrentDirection){
        case 1:{
            // Le serpent monte vers le haut
            if( IndexRow.at(0) == 1 ){
                // On est sur la première ligne
                IndexRow.at(0) == grid.getNumberLines();
            }else{
                // On est sur une ligne quelconque
                IndexRow.at(0) -= 1;
            }
        }
        case 2:{
            // Le serpent va vers la droite
            if( IndexColumn.at(0) == grid.getNumberColumns() ){
                // On est sur la dernière colonne
                IndexColumn.at(0) == 1;
            }else{
                // On est sur une colonne quelconque
                IndexColumn.at(0) += 1;
            }
        }
        case 3:{
            // Le serpent va vers le bas
            if( IndexRow.at(0) == grid.getNumberLines() ){
                // On est sur la dernière ligne
                IndexRow.at(0) == 1;
            }else{
                // On est sur une ligne quelconque
                IndexRow.at(0) += 1;
            }
        }
        case 4:{
            // Le serpent va vers la gauche
            if( IndexColumn.at(0) == 1 ){
                // On est sur la dernière colonne
                IndexColumn.at(0) == grid.getNumberColumns();
            }else{
                // On est sur une colonne quelconque
                IndexColumn.at(0) -= 1;
            }
        }
    }
    // On actualise le rectangle de la tête 
    SnakeRect.at(0) = {grid.getOffsetXLeft()+(IndexColumn.at(0)-1)*(grid.getCellWidth()+1)+3, grid.getOffsetYTop() + (IndexRow.at(0)-1)*(grid.getCellHeight()+1)+3, grid.getOffsetXLeft() + IndexColumn.at(0)*(grid.getCellWidth()+1)-1, grid.getOffsetYTop() + IndexRow.at(0)*(grid.getCellHeight()+1)-1};

    // On actualise la queue du serpent
    for(i=1; i<Size; i++){
        columnTemp = IndexColumn.at(i);
        rowTemp = IndexRow.at(i);
        rectTemp = SnakeRect.at(i);
        IndexColumn.at(i) = columnOld;
        IndexRow.at(i) = rowOld;
        SnakeRect.at(i) = rectOld;
        columnOld = columnTemp;
        rowOld = rowTemp;
        rectOld = rectTemp;
    }
}
void Snake::grow(){
    // Fonction qui permet de faire grandir le serpent de 1
    IndexColumn.push_back(IndexColumn.at(Size-1));
    IndexRow.push_back(IndexRow.at(Size-1));
    SnakeRect.push_back(SnakeRect.back());
    Size += 1;
}
void Snake::shrink(){
    // Fonction qui permet de diminuer le serpent de 1
    IndexColumn.pop_back();
    IndexRow.pop_back();
    SnakeRect.pop_back();
    Size-=1;
}
void Snake::init(const Grid& grid){
    // Fonction qui réinitialise le serpent aux paramètres initiaux
    Speed = 100;
    IndexColumn.erase(IndexColumn.begin(),IndexColumn.begin()+Size);
    IndexRow.erase(IndexRow.begin(),IndexRow.begin()+Size);
    IndexColumn.push_back(ceil(static_cast<double> (grid.getNumberColumns()/2)));
    IndexRow.push_back(ceil(static_cast<double> (grid.getNumberLines()/2)));
    SnakeRect.erase(SnakeRect.begin(),SnakeRect.begin()+Size);
    SnakeRect.push_back({grid.getOffsetXLeft()+(IndexColumn.at(0)-1)*(grid.getCellWidth()+1)+2, grid.getOffsetYTop() + (IndexRow.at(0)-1)*(grid.getCellHeight()+1)+2, grid.getOffsetXLeft() + IndexColumn.at(0)*(grid.getCellWidth()+1)-1, grid.getOffsetYTop() + IndexRow.at(0)*(grid.getCellHeight()+1)-1});
    CurrentDirection = 2;
    PreviousDirection = 2;
    Size = 1;
}
void Snake::gridChanged(const Grid& grid){
    // Fonction qui actualise le serpent lorsque la grille change

    for(int i = 0; i <Size; i++){
        SnakeRect.at(i) = {grid.getOffsetXLeft()+(IndexColumn.at(i)-1)*(grid.getCellWidth()+1)+2, grid.getOffsetYTop() + (IndexRow.at(i)-1)*(grid.getCellHeight()+1)+2, grid.getOffsetXLeft() + IndexColumn.at(i)*(grid.getCellWidth()+1)-1, grid.getOffsetYTop() + IndexRow.at(i)*(grid.getCellHeight()+1)-1};
    }
}

// Getters
int Snake::getSpeed() const {
    return Speed;
}
const std::vector<int>& Snake::getIndexColumn() const {
    return IndexColumn;
}
const std::vector<int>& Snake::getIndexRow() const {
    return IndexRow;
}
int Snake::getCurrentDirection() const {
    return CurrentDirection;
}
int Snake::getPreviousDirection() const {
    return PreviousDirection;
}
int Snake::getSize() const {
    return Size;
}
const std::vector<RECT>& Snake::getSnakeRect() const {
    return SnakeRect;
}

// Setters
void Snake::setSpeed(int speed) {
    Speed = speed;
}
void Snake::setIndexColumn(const std::vector<int>& indexColumn) {
    IndexColumn = indexColumn;
}
void Snake::setIndexRow(const std::vector<int>& indexRow) {
    IndexRow = indexRow;
}
void Snake::setCurrentDirection(int currentDirection) {
    CurrentDirection = currentDirection;
}
void Snake::setPreviousDirection(int previousDirection) {
    PreviousDirection = previousDirection;
}
void Snake::setSize(int size) {
    Size = size;
}
void Snake::setSnakeRect(const std::vector<RECT>& snakeRect){
    SnakeRect = snakeRect;
}