#include <iostream>
#include "distanceLevenshtein.h"


int main(){
    std::string s = "bitten";
    std::string t = "sitting";
    std::string u = "ecoles";
    std::string v = "eclose";

    // Appel avec mémoïsation
    std::cout << "Distance de Levenshtein (avec mémoïsation) : " << levenshtein(s, t, true) << std::endl;

    // Appel sans mémoïsation
    std::cout << "Distance de Levenshtein (sans mémoïsation) : " << levenshtein(s, t, false) << std::endl;

    // Appel avec la méthode itérative
    std::cout << "Distance de Levenshtein (méthode itérative) : " << levenshtein_iterative(s, t,true) << std::endl;

    // Calcul de la matrice des distances (Levenshtein standard)
    auto dp_standard = levenshtein_matrix(u, v, false);
    std::cout << "Matrice des distances (Levenshtein standard) : " << std::endl;
    print_matrix(dp_standard, u, v);

    // Affichage du chemin Levenshtein (standard)
    std::cout << "Affichage du chemin Levenshtein (standard) : " << std::endl;
    print_levenshtein_path(u, v, dp_standard, false);

    // Calcul de la matrice des distances (Damerau-Levenshtein activé)
    auto dp_damerau = levenshtein_matrix(u, v, true);
    std::cout << "Matrice des distances (Damerau-Levenshtein) : " << std::endl;
    print_matrix(dp_damerau, u, v);

    // Affichage du chemin Levenshtein (Damerau-Levenshtein activé)
    std::cout << "Affichage du chemin Damerau-Levenshtein : " << std::endl;
    print_levenshtein_path(u, v, dp_damerau, true);

    
    // Affichage du chemin Damerau-Levenshtein version itérative linéaire
    int distance = damerau_levenshtein_linear_mem(s, t);
    std::cout << "Distance Damerau-Levenshtein (linéaire en espace) : " << distance << std::endl;

    // Comparaison du temps mis par les algos
    measure_execution_times(u, v);
    
    return 0;
}

