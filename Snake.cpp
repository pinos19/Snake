#include "Snake.h"


// Constructeurs
Snake::Snake()
    :Speed{0},IndexColumn{},IndexRow{},Directions{},Size{0},SnakeRect{},SnakeColor{RGB(255,255,255)}, SnakePreviousRect{0,0,0,0}{}
Snake::Snake(int speed, std::vector<int> indexColumn, std::vector<int> indexRow, std::vector<int> directions, int size, std::vector<RECT> snakeRect, COLORREF snakeColor, RECT snakePreviousRect)
    :Speed{speed}, IndexColumn{indexColumn}, IndexRow{indexRow}, Directions{directions}, Size{size}, SnakeRect{snakeRect}, SnakeColor{snakeColor}, SnakePreviousRect{snakePreviousRect}{}

// Méthodes de la classe
int Snake::move(const Grid& grid){
    // Fonction qui permet de déplacer le serpent avec la valeur de déplacement actuelle
    // La fonction retourne un entier qui permet d'indiquer l'état de la nouvelle case
    // 0 => vide
    // bombe = 1
    // clous = 2
    // poussière = 3
    // serpent = 4
    int columnOld = IndexColumn.at(0);
    int rowOld = IndexRow.at(0);
    int columnTemp{0}, rowTemp{0}, i{0}, newRow{rowOld}, newColumn{columnOld};
    RECT rectOld = SnakeRect.at(0);
    RECT rectTemp {0,0,0,0};
    int valueCell = 0;

    // Avant le déplacement on récupère le dernier rectangle
    SnakePreviousRect = SnakeRect.back();

    // On actualise la tête du serpent
    switch(Directions.front()){
        case 1:{
            // Le serpent monte vers le haut
            if( rowOld == 1 ){
                // On est sur la première ligne
                newRow = grid.getNumberLines();
            }else{
                // On est sur une ligne quelconque
                newRow -= 1;
            }
            break;
        }
        case 2:{
            // Le serpent va vers la droite
            if( columnOld == grid.getNumberColumns() ){
                // On est sur la dernière colonne
                newColumn = 1;
            }else{
                // On est sur une colonne quelconque
                newColumn += 1;
            }
            break;
        }
        case 3:{
            // Le serpent va vers le bas
            if( rowOld == grid.getNumberLines() ){
                // On est sur la dernière ligne
                newRow = 1;
            }else{
                // On est sur une ligne quelconque
                newRow += 1;
            }
            break;
        }
        case 4:{
            // Le serpent va vers la gauche
            if( columnOld == 1 ){
                // On est sur la dernière colonne
                newColumn = grid.getNumberColumns();
            }else{
                // On est sur une colonne quelconque
                newColumn -= 1;
            }
            break;
        }
    }
    // On regarde si la case suivante n'est pas une case serpent
    if( isSnake(newRow, newColumn) ){
        valueCell = 4;
    }else{
        // On teste pour voir si le serpent n'est pas sur une bombe, un clou ou de la poussière
        valueCell = grid.checkGrid(newRow, newColumn);
    }

    // On affecte les nouvelles valeurs
    IndexColumn.at(0) = newColumn;
    IndexRow.at(0) = newRow;

    // On actualise le rectangle de la tête 
    SnakeRect.at(0) = {grid.getOffsetXLeft()+(IndexColumn.at(0)-1)*(grid.getCellWidth()+1)+2, grid.getOffsetYTop() + (IndexRow.at(0)-1)*(grid.getCellHeight()+1)+2, grid.getOffsetXLeft() + IndexColumn.at(0)*(grid.getCellWidth()+1)-1, grid.getOffsetYTop() + IndexRow.at(0)*(grid.getCellHeight()+1)-1};

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

    return valueCell;
}
bool Snake::isSnake(int rowIndex, int columnIndex){
    // Fonction qui permet de dire si la case ciblée cible une case
    // serpent ou non
    bool isSnake {false};

    for(int i =0; i<Size; i++){
        if( IndexColumn.at(i) == columnIndex && IndexRow.at(i) == rowIndex){
            isSnake = true;
            break;
        }
    }

    return isSnake;
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
    Speed = 50;
    IndexColumn.erase(IndexColumn.begin(),IndexColumn.begin()+Size);
    IndexRow.erase(IndexRow.begin(),IndexRow.begin()+Size);
    IndexColumn.push_back(ceil(static_cast<double> (grid.getNumberColumns()/2)));
    IndexRow.push_back(ceil(static_cast<double> (grid.getNumberLines()/2)));
    SnakeRect.erase(SnakeRect.begin(),SnakeRect.begin()+Size);
    SnakeRect.push_back({grid.getOffsetXLeft()+(IndexColumn.at(0)-1)*(grid.getCellWidth()+1)+2, grid.getOffsetYTop() + (IndexRow.at(0)-1)*(grid.getCellHeight()+1)+2, grid.getOffsetXLeft() + IndexColumn.at(0)*(grid.getCellWidth()+1)-1, grid.getOffsetYTop() + IndexRow.at(0)*(grid.getCellHeight()+1)-1});
    Directions.push_back(2);
    SnakeColor = RGB(255,255,255);
    SnakePreviousRect = SnakeRect.back();
    Size = 1;
}
void Snake::gridChanged(const Grid& grid){
    // Fonction qui actualise le serpent lorsque la grille change

    for(int i = 0; i <Size; i++){
        SnakeRect.at(i) = {grid.getOffsetXLeft()+(IndexColumn.at(i)-1)*(grid.getCellWidth()+1)+2, grid.getOffsetYTop() + (IndexRow.at(i)-1)*(grid.getCellHeight()+1)+2, grid.getOffsetXLeft() + IndexColumn.at(i)*(grid.getCellWidth()+1)-1, grid.getOffsetYTop() + IndexRow.at(i)*(grid.getCellHeight()+1)-1};
    }
}
void Snake::addDirection(int direction){
    Directions.push_back(direction);
}
bool Snake::popDirection() {
    if (Directions.empty()) {
        return false;
    }
    if( Directions.size() > 1 ){
        Directions.erase(Directions.begin());
    }
    return true;
}
bool Snake::peekDirection(int &direction) const {
    if (Directions.empty()) {
        return false;
    }
    direction = Directions.front();
    return true;
}

// Getters
int Snake::getSpeed() const { return Speed;}
const std::vector<int>& Snake::getIndexColumn() const { return IndexColumn; }
const std::vector<int>& Snake::getIndexRow() const { return IndexRow; }
int Snake::getSize() const { return Size; }
const std::vector<RECT>& Snake::getSnakeRect() const { return SnakeRect; }
COLORREF Snake::getSnakeColor() const{ return SnakeColor; }
const RECT& Snake::getSnakePreviousRect() const{ return SnakePreviousRect; }
const std::vector<int>& Snake::getDirections() const{ return Directions; }

// Setters
void Snake::setSpeed(int speed) { Speed = speed; }
void Snake::setIndexColumn(const std::vector<int>& indexColumn) { IndexColumn = indexColumn; }
void Snake::setIndexRow(const std::vector<int>& indexRow) { IndexRow = indexRow; }
void Snake::setSize(int size) { Size = size; }
void Snake::setSnakeRect(const std::vector<RECT>& snakeRect){ SnakeRect = snakeRect; }
void Snake::setSnakeColor(COLORREF snakeColor){ SnakeColor = snakeColor; }
void Snake::setSnakePreviousRect(const RECT& snakePreviousRect){ SnakePreviousRect = snakePreviousRect; }
void Snake::setDirections(const std::vector<int>& directions){ Directions = directions; }