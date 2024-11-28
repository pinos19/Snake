#include "Snake.h"


// Constructeurs
Snake::Snake()
    :Speed{0},IndexColumn{},IndexRow{},CurrentDirection{0},PreviousDirection{0},Size{0}{}
Snake::Snake(double speed, std::vector<int> indexColumn, std::vector<int> indexRow, int currentDirection, int previousDirection, int size)
    :Speed{speed}, IndexColumn{indexColumn}, IndexRow{indexRow}, CurrentDirection{currentDirection}, PreviousDirection{previousDirection}, Size{size}{}

// Méthodes de la classe 
void Snake::move(const Grid& grid){
    // Fonction qui permet de déplacer le serpent avec la valeur de déplacement actuelle
    
}
void Snake::grow(){
}
void Snake::shrink(){
}
void Snake::init(){
}

// Getters
double Snake::getSpeed() const {
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

// Setters
void Snake::setSpeed(double speed) {
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