#include "Grid.h"


// Constructeurs
Grid::Grid()
    :RatioCell{0},
    NumberLines{0},
    NumberColumns{0},
    OffsetXLeft{0},
    OffsetXRight{0},
    OffsetYTop{0},
    OffsetYBottom{0}, 
    GridColor{RGB(255,255,255)},
    IndexBombs{},
    IndexNails{},
    IndexDusts{}{}
Grid::Grid(int ratioCell,int numberLines,int numberColumns,int offsetXLeft,int offsetXRight,int offsetYTop,int offsetYBottom,COLORREF gridColor,std::vector<int> indexBombs,std::vector<int> indexNails,std::vector<int> indexDusts)
    :RatioCell{ratioCell},
    NumberLines{numberLines},
    NumberColumns{numberColumns},
    OffsetXLeft{offsetXLeft},
    OffsetXRight{offsetXRight},
    OffsetYTop{offsetYTop},
    OffsetYBottom{offsetYBottom},
    GridColor{gridColor},
    IndexBombs{indexBombs},
    IndexNails{indexNails},
    IndexDusts{indexDusts}{}

// Méthodes de la classe
void Grid::windowChanged(int widthWindow, int heightWindow){
    // Fonction qui permet de mettre à jour la grille.

    // On calcule le cell width à partir du nombre de colonnes et de lignes
    CellWidth = (widthWindow-NumberColumns-1)/NumberColumns;
    CellHeight = (heightWindow-NumberLines-1)/NumberLines;

    // Calculer le décalage pour centrer la grille
    OffsetXLeft = (widthWindow % CellWidth) / 2;
    OffsetXRight = ceil(static_cast<double> (widthWindow % CellWidth)/2);
    OffsetYTop = (heightWindow % CellHeight) / 2;
    OffsetYBottom = ceil(static_cast<double> (heightWindow % CellHeight)/2);
}
void Grid::init(int widthWindow, int heightWindow){
    // Initialisation de la première grille en fonction de la largeur et hauteur de la grille

    // Ratio de la grille 
    RatioCell = 60;
    
    CellWidth = std::max(widthWindow, heightWindow)/RatioCell;
    CellHeight = CellWidth;
    NumberLines = heightWindow/(CellHeight+1);
    NumberColumns = widthWindow/(CellWidth+1);
    int offsetX = widthWindow-CellWidth*NumberColumns-NumberColumns-1;
    int offsetY = heightWindow-CellHeight*NumberLines-NumberLines-1;

    while(offsetY < 0){
        NumberLines--;
        offsetY = heightWindow-CellHeight*NumberLines-NumberLines-1;
    }
    while(offsetX < 0){
        NumberColumns--;
        offsetX = widthWindow-CellWidth*NumberColumns-NumberColumns-1;
    }

    // Calculer le décalage pour centrer la grille
    OffsetXLeft = offsetX/2;
    OffsetXRight = ceil(static_cast<double> (offsetX)/2);
    OffsetYTop = offsetY/2;
    OffsetYBottom = ceil(static_cast<double> (offsetY)/2);

    // Couleur grille
    GridColor = RGB(255,255,255);

    // Initialisation nulle des index de clous, bombes et poussières
    IndexBombs = {};
    IndexNails = {};
    IndexDusts = {};
}
int Grid::checkGrid(int rowIndex, int columnIndex) const{
    // Fonction qui prend en entrée un index de ligne et de colonne. La fonction retourne une valeur en fonction
    // de la case.
    // case :
    // vide = 0
    // bombe = 1
    // clous = 2
    // poussière = 3
    std::vector<int>::const_iterator  it;
    int valueGrid = 0;

    // La première chose est de convertir les deux index en un seul index
    int cellIndex = (columnIndex-1)*NumberLines + rowIndex;

    // Ensuite il faut comparer cet index avec les trois tableaux
    if( !IndexBombs.empty()){
        it = std::find(IndexBombs.begin(), IndexBombs.end(), 1);
        if( it != IndexBombs.end() ){
            valueGrid = 1;
        }
    }
    if( !IndexNails.empty()){
        it = std::find(IndexNails.begin(), IndexNails.end(), 2);
        if( it != IndexNails.end() ){
            valueGrid = 2;
        }
    }
    if( !IndexDusts.empty()){
        it = std::find(IndexDusts.begin(), IndexDusts.end(), 3);
        if( it != IndexDusts.end() ){
            valueGrid = 3;
        }
    }

    return valueGrid;
}

// Getters
int Grid::getRatioCell() const { return RatioCell; }
int Grid::getNumberLines() const { return NumberLines; }
int Grid::getNumberColumns() const { return NumberColumns; }
int Grid::getOffsetXLeft() const { return OffsetXLeft; }
int Grid::getOffsetXRight() const { return OffsetXRight; }
int Grid::getOffsetYTop() const { return OffsetYTop; }
int Grid::getOffsetYBottom() const { return OffsetYBottom; }
int Grid::getCellWidth() const{ return CellWidth; }
int Grid::getCellHeight() const{ return CellHeight; }
COLORREF Grid::getGridColor() const{ return GridColor; }
const std::vector<int>& Grid::getIndexBombs() const { return IndexBombs; }
const std::vector<int>& Grid::getIndexNails() const { return IndexNails; }
const std::vector<int>& Grid::getIndexDusts() const { return IndexDusts; }

// Setters
void Grid::setRatioCell(int ratioCell){ RatioCell = ratioCell; }
void Grid::setNumberLines(int numberLines) { NumberLines = numberLines; }
void Grid::setNumberColumns(int numberColumns) { NumberColumns = numberColumns; }
void Grid::setOffsetXLeft(int offsetXLeft) { OffsetXLeft = offsetXLeft; }
void Grid::setOffsetXRight(int offsetXRight) { OffsetXRight = offsetXRight; }
void Grid::setOffsetYTop(int offsetYTop) { OffsetYTop = offsetYTop; }
void Grid::setOffsetYBottom(int offsetYBottom) { OffsetYBottom = offsetYBottom; }
void Grid::setCellWidth(int cellWidth){ CellWidth = cellWidth; }
void Grid::setCellHeight(int cellHeight){ CellHeight = cellHeight; }
void Grid::setGridColor(COLORREF gridColor){ GridColor = gridColor; }
void Grid::setIndexBombs(const std::vector<int>& indexBombs) { IndexBombs = indexBombs; }
void Grid::setIndexNails(const std::vector<int>& indexNails) { IndexNails = indexNails; }
void Grid::setIndexDusts(const std::vector<int>& indexDusts) { IndexDusts = indexDusts; }