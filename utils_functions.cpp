#include "utils_functions.h"


int minVector(const std::vector<int>& vector){
    // Fonction qui retourne la valeur minimale du vecteur donné en entrée
    int min {0};

    if( !vector.empty() ){
        min = vector.at(0);
        for(int i = 1; i < vector.size(); i++){
            if( vector.at(i) < min){
                min = vector.at(i);
            }
        }
    }
    return min;
}

int maxVector(const std::vector<int>& vector){
    // Fonction qui retourne la valeur maximale du vecteur donné en entrée
    int max {0};

    if( !vector.empty() ){
        max = vector.at(0);
        for(int i = 1; i < vector.size(); i++){
            if( vector.at(i) > max){
                max = vector.at(i);
            }
        }
    }
    return max;
}