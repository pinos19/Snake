#include "Grid.h"


// Constructeurs
Grid::Grid()
    :RatioCell{},NumberLines{},NumberColumns{},OffsetXLeft{},OffsetXRight{},OffsetYTop{},OffsetYBottom{}{}
Grid::Grid(double ratioCell,double numberLines,double numberColumns,double offsetXLeft,double offsetXRight,double offsetYTop,double offsetYBottom)
    :RatioCell{ratioCell},NumberLines{numberLines},NumberColumns{numberColumns},OffsetXLeft{offsetXLeft},OffsetXRight{offsetXRight},OffsetYTop{offsetYTop},OffsetYBottom{offsetYBottom}{}

