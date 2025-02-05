#include <iostream>
#include "matrice.h"
#include <cstdlib>
#include <ctime>


int main() {
    // Création d'une matrice de 3x3
    Matrice mat(3, 3);

    // Modification et affichage
    mat.setElement(0, 0, 1);
    mat.setElement(0, 1, 2);
    mat.setElement(0, 2, 3);
    mat.setElement(1, 0, 4);
    mat.setElement(1, 1, 5);
    mat.setElement(1, 2, 6);
    mat.setElement(2, 0, 7);
    mat.setElement(2, 1, 8);
    mat.setElement(2, 2, 9);

    mat.afficher();
    
    srand(time(0));  // Initialiser le générateur de nombres aléatoires

    const int n = 3;  // Nombre de matrices à générer
    const int taille = 3;  // Taille des matrices carrées

    for (int i = 0; i < n; i++) {
        std::cout << "Génération de la matrice A" << i + 1 << " :\n";
        
        // Créer une matrice aléatoire
        Matrice A(taille, taille);
        A.remplirAleatoire(0, 10);  // Remplir avec des valeurs aléatoires entre 0 et 10
        A.afficher();

        // Copier la matrice
        std::cout << "Copie de la matrice A" << i + 1 << " vers B" << i + 1 << " :\n";
        Matrice B(A);  // Utilisation du constructeur par copie
        B.afficher();

        // Multiplier les matrices
        std::cout << "Multiplication de A" << i + 1 << " * B" << i + 1 << " :\n";
        Matrice C = A * B;
        C.afficher();

        std::cout << "--------------------------------------------\n";
    }

    return 0;
}
