#include "Grid.h"


// Methods
void Grid::init(int widthWindow, int heightWindow){
    // Initialization of the grid

    // Ratio of the grid
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
    IndexBombs.clear();
    IndexNails.clear();
    IndexDusts.clear();
}
void Grid::windowChanged(int widthWindow, int heightWindow){
    // Function which actualize the grid because the window changed

    // Compute the cell width and height
    CellWidth = (widthWindow-NumberColumns-1)/NumberColumns;
    CellHeight = (heightWindow-NumberLines-1)/NumberLines;

    // Compute the offsets to center the grid
    OffsetXLeft = (widthWindow % CellWidth) / 2;
    OffsetXRight = ceil(static_cast<double> (widthWindow % CellWidth)/2);
    OffsetYTop = (heightWindow % CellHeight) / 2;
    OffsetYBottom = ceil(static_cast<double> (heightWindow % CellHeight)/2);
}
Grid::TileContent Grid::getContentTile(int rowIndex, int ColumnIndex, const Snake &snake){
    // Function which check the content of a cell
    // Return the content, TileContent

    // Beginning with the snake, we check if the cell does not contain a part of the snake
    const std::vector<int> snakeIndexRow = snake.getIndexRow();
    const std::vector<int> snakeIndexColumn = snake.getIndexColumn();

    std::vector<int>::const_iterator it1 = std::find(snakeIndexRow.cbegin(), snakeIndexRow.cend(), rowIndex);
    if( it1 != snakeIndexRow.cend() ){
        if( std::find(snakeIndexColumn.cbegin(), snakeIndexColumn.cend(), ColumnIndex) != snakeIndexColumn.cend() ){
            return TileContent::Snake;
        }
    }

    // Now we are going to check the other possibilities
    std::pair<int, int> cellPair {rowIndex, ColumnIndex};
    if( !IndexBombs.empty()){
        if( std::find(IndexBombs.begin(), IndexBombs.end(), cellPair) != IndexBombs.end() ){
            return TileContent::Bomb;
        }
    }
    if( !IndexNails.empty()){
        if( std::find(IndexNails.begin(), IndexNails.end(), cellPair) != IndexNails.end() ){
            return TileContent::Nail;
        }
    }
    if( !IndexDusts.empty()){
        if( std::find(IndexDusts.begin(), IndexDusts.end(), cellPair) != IndexDusts.end() ){
            return TileContent::Dust;
        }
    }
    return TileContent::Empty;
}
Grid::TileContent Grid::getContentTile(int rowIndex, int ColumnIndex, std::list<std::pair<int, int>>::iterator &it){
    // Function which check the content of a cell
    // Return the content, TileContent

    // Now we are going to check the other possibilities
    std::pair<int, int> cellPair {rowIndex, ColumnIndex};
    if( !IndexBombs.empty()){
        it = std::find(IndexBombs.begin(), IndexBombs.end(), cellPair);
        if( it != IndexBombs.end() ){
            return TileContent::Bomb;
        }
    }
    if( !IndexNails.empty()){
        it = std::find(IndexNails.begin(), IndexNails.end(), cellPair);
        if( it != IndexNails.end() ){
            return TileContent::Nail;
        }
    }
    if( !IndexDusts.empty()){
        it = std::find(IndexDusts.begin(), IndexDusts.end(), cellPair);
        if( it != IndexDusts.end() ){
            return TileContent::Dust;
        }
    }
    return TileContent::Empty;
}
bool Grid::popGrid(int rowIndex, int columnIndex){
    // Function which delete the element of a cell and return true or false
    // True if there was an element in the cell, false otherwise

    std::list<std::pair<int, int>>::iterator iterator;

    TileContent tileContent = getContentTile(rowIndex, columnIndex, iterator);

    switch( tileContent ){
        case TileContent::Bomb:{
            IndexBombs.erase(iterator);
            return true;
        }
        case TileContent::Nail:{
            IndexNails.erase(iterator);
            return true;
        }
        case TileContent::Dust:{
            IndexDusts.erase(iterator);
            return true;
        }
    }

    return false;
}
void Grid::fillGridWithElements(const std::vector<int>& indexToAvoid){
    // Fonction qui remet à zéro les vecteurs d'index qui contiennent les clous, les poussières
    // et les bombes

    // Remise à zéro
    IndexBombs.clear();
    IndexNails.clear();
    IndexDusts.clear();

    // On va peupler avec des statistiques les 3 éléments sur toute la grille puis on retirera 
    // les éléments qui se trouvent sur les index donnés en entrée
    // probabilité élément = 10%;
    // probabilité bombe sachant élément = 20%;
    // probabilité clou sachant élément = 30%;
    // probabilité poussière sachant élément = 50%;
    int numberCells = NumberColumns*NumberLines;

    // Modèle de génération de nombre aléatoire entre 1 et 10
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(1,10);
    int typeElement {0};
    std::vector<int>::const_iterator it;

    for(int i=1; i<=numberCells; i++){
        if( dist(gen) == 1 ){
            // Il y a un élément, on regarde si le numéro de cellule est 
            // dans la zone immunisée ou non
            it = find(indexToAvoid.begin(), indexToAvoid.end(), i);

            if( it == indexToAvoid.end() ){
                // Index pas dans la zonne d'immunité, on ressort un nombre alétoire pour décider
                // le type d'élément
                typeElement = dist(gen);
                if( typeElement >= 9 ){
                    // 20 % bombe
                    IndexBombs.push_back(i);
                }else if( typeElement >= 6){
                    // 30 % clous
                    IndexNails.push_back(i);
                }else{
                    // 50 % poussière
                    IndexDusts.push_back(i);
                }
            }
        }
    }
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
const std::vector<std::pair<int, int>>& Grid::getIndexBombs() const { return IndexBombs; }
const std::vector<std::pair<int, int>>& Grid::getIndexNails() const { return IndexNails; }
const std::vector<std::pair<int, int>>& Grid::getIndexDusts() const { return IndexDusts; }

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
void Grid::setIndexBombs(const std::vector<std::pair<int, int>>& indexBombs) { IndexBombs = indexBombs; }
void Grid::setIndexNails(const std::vector<std::pair<int, int>>& indexNails) { IndexNails = indexNails; }
void Grid::setIndexDusts(const std::vector<std::pair<int, int>>& indexDusts) { IndexDusts = indexDusts; }