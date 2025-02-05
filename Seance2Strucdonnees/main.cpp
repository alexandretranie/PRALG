#include <iostream>
#include "IntTree.h"



int main() {
    // Création de la racine de l'arbre avec la valeur 12
    IntTree* root = new IntTree(12);

    // Ajout de l'enfant 8 à la racine
    root->addAsLastChild(new IntTree(8));

    // Ajout des enfants 4 et 9 à l'enfant 8
    root->getChild(0)->addAsLastChild(new IntTree(4));
    root->getChild(0)->addAsLastChild(new IntTree(9));

    // Ajout de l'enfant 23 à la racine
    root->addAsLastChild(new IntTree(23));

    // Ajout de l'enfant 17 à l'enfant 23
    root->getChild(1)->addAsLastChild(new IntTree(17));

    // Ajout de l'enfant 15 à l'enfant 17
    root->getChild(1)->getChild(0)->addAsLastChild(new IntTree(15));

    root->display("* ");

    // Calculer la profondeur maximale de l'arbre
    int depthmax = root->maxDepth();
    std::cout << "La profondeur maximale de l'arbre est : " << depthmax << std::endl;

    // Calculer la profondeur minimale de l'arbre
    int depthmin = root->minDepth();
    std::cout << "La profondeur minimale de l'arbre est : " << depthmin << std::endl;



    // Libération de la mémoire allouée pour l'arbre
    delete root;
    return 0;
}



/*
int main() {
    try {
        // Création d'un arbre simple
        IntTree* root = new IntTree(10);

        // 1. Test de getChild avec un index hors limites
        try {
            root->getChild(0);  // Il n'y a pas encore d'enfant, donc cela devrait lever une exception
        } catch (const std::out_of_range& e) {
            std::cout << "Erreur (getChild) : " << e.what() << std::endl;
        }

        // 2. Test de setChild avec un index hors limites
        try {
            root->setChild(0, new IntTree(20));  // Aucun enfant à l'index 0
        } catch (const std::out_of_range& e) {
            std::cout << "Erreur (setChild, index hors limites) : " << e.what() << std::endl;
        }

        // 3. Test de setChild avec un pointeur nul
        try {
            root->addAsLastChild(new IntTree(15));  // Ajout d'un enfant valide
            root->setChild(0, nullptr);  // Tentative d'ajouter un enfant nul
        } catch (const std::invalid_argument& e) {
            std::cout << "Erreur (setChild, newChild == nullptr) : " << e.what() << std::endl;
        }

        // 4. Test de addAsLastChild avec un pointeur nul
        try {
            root->addAsLastChild(nullptr);  // Tentative d'ajouter un enfant nul
        } catch (const std::invalid_argument& e) {
            std::cout << "Erreur (addAsLastChild, newChild == nullptr) : " << e.what() << std::endl;
        }

        // 5. Test de removeLastChild sur un arbre sans enfants
        IntTree* emptyNode = new IntTree(5);  // Création d'un nœud sans enfants
        bool result = emptyNode->removeLastChild();  // Tentative de suppression
        if (!result) {
            std::cout << "Suppression échouée (removeLastChild) : pas d'enfants à supprimer" << std::endl;
        }

        // Nettoyage de la mémoire
        delete root;
        delete emptyNode;

    } catch (const std::exception& e) {
        std::cout << "Erreur imprévue : " << e.what() << std::endl;
    }

    return 0;
}
*/
