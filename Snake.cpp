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
    // On actualise la queue du serpent
    for(i=1; i<Size; i++){
        columnTemp = IndexColumn.at(i);
        rowTemp = IndexRow.at(i);
        IndexColumn.at(i) = columnOld;
        IndexRow.at(i) = rowOld;
        columnOld = columnTemp;
        rowOld = rowTemp;
    }
}
void Snake::grow(){
    // Fonction qui permet de faire grandir le serpent de 1
    IndexColumn.push_back(IndexColumn.at(Size-1));
    IndexRow.push_back(IndexRow.at(Size-1));
    Size += 1;
}
void Snake::shrink(){
    // Fonction qui permet de diminuer le serpent de 1
    IndexColumn.pop_back();
    IndexRow.pop_back();
    Size-=1;
}
void Snake::init(const Grid& grid){
    // Fonction qui réinitialise le serpent aux paramètres initiaux
    Speed = 100;
    IndexColumn.erase(IndexColumn.begin(),IndexColumn.begin()+Size);
    IndexRow.erase(IndexRow.begin(),IndexRow.begin()+Size);
    IndexColumn.push_back(ceil(static_cast<double> (grid.getNumberColumns()/2)));
    IndexRow.push_back(ceil(static_cast<double> (grid.getNumberLines()/2)));
    CurrentDirection = 2;
    PreviousDirection = 2;
    Size = 1;
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