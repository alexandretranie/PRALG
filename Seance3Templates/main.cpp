#include "eleves.h"
#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    srand(static_cast<unsigned>(time(0))); // Initialisation du générateur aléatoire

    std::vector<Eleve> eleves;
    int nombreEleves = 10; // Nombre d'élèves à générer

    // Remplir le vecteur avec des élèves aléatoires
    remplirEleves(eleves, nombreEleves);

    // Afficher les informations avant le tri
    std::cout << "Avant le tri:" << std::endl;
    for (const auto& eleve : eleves) {
        eleve.afficherInfo();
    }

    // Initialiser le compteur de comparaisons
    int compteur = 0;

    // Passer le compteur via le constructeur
    CompareNom cmpNom(&compteur);

    // Trier par nom en utilisant le foncteur CompareNom avec compteur
    std::sort(eleves.begin(), eleves.end(), cmpNom);

    // Afficher les informations après le tri par nom
    std::cout << "\nAprès le tri par nom (ordre alphabétique):" << std::endl;
    for (std::vector<Eleve>::iterator it = eleves.begin(); it != eleves.end(); ++it) {
        it->afficherInfo();
    }

    // Afficher le nombre de comparaisons pour le tri par nom
    std::cout << "Nombre de comparaisons pour le tri par nom: " << compteur << std::endl;

    // Réinitialiser le compteur
    compteur = 0;

    // Passer le compteur via le constructeur pour CompareNote
    CompareNote cmpNote(&compteur);

    // Trier par note décroissante en utilisant le foncteur CompareNote avec compteur
    std::sort(eleves.begin(), eleves.end(), cmpNote);

    // Afficher les informations après le tri par note décroissante
    std::cout << "\nAprès le tri par note décroissante:" << std::endl;
    for (std::vector<Eleve>::iterator it = eleves.begin(); it != eleves.end(); ++it) {
        it->afficherInfo();
    }

    // Afficher le nombre de comparaisons pour le tri par note
    std::cout << "Nombre de comparaisons pour le tri par note: " << compteur << std::endl;

    return 0;
}
