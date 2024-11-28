#include "Snake.h"


// Constructeurs
Snake::Snake()
    :Speed{0},IndexColumn{},IndexRow{},CurrentDirection{0},PreviousDirection{0},Size{0}{}
Snake::Snake(double speed, std::vector<int> indexColumn, std::vector<int> indexRow, int currentDirection, int previousDirection, int size)
    :Speed{speed}, IndexColumn{indexColumn}, IndexRow{indexRow}, CurrentDirection{currentDirection}, PreviousDirection{previousDirection}, Size{size}{}

// Méthodes de la classe 
void Snake::move(Grid grid){

}
void Snake::grow(){

}
void Snake::shrink(){

}
void Snake::init(){

    
}