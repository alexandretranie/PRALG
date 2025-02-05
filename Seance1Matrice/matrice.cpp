#include "matrice.h"
#include <iostream>
#include <cassert>
#include <cstdlib>  
#include <ctime>   

// Constructeur
Matrice::Matrice(int l, int c) : lignes(l), colonnes(c) {
    tab = new int[lignes * colonnes];  // Allocation dynamique du tableau
    compteur = new int(1);  // Initialisation du compteur à 1
    std::cout << "Constructeur appelé : Matrice de " << lignes << "x" << colonnes << " créée." << std::endl;
}

// Constructeur par copie
Matrice::Matrice(const Matrice& autre) : lignes(autre.lignes), colonnes(autre.colonnes), tab(autre.tab), compteur(autre.compteur) {
    (*compteur)++;  // Incrémentation du compteur pour indiquer que le tableau est partagé
    std::cout << "Constructeur par copie appelé : Matrice partagée, compteur = " << *compteur << std::endl;
}

// Destructeur
Matrice::~Matrice() {
    (*compteur)--;  // Décrémentation du compteur
    if (*compteur == 0) {
        delete[] tab;  // Libération de la mémoire uniquement si aucun autre objet ne partage le tableau
        delete compteur;  // Libération du compteur
        std::cout << "Destructeur appelé : Matrice détruite." << std::endl;
    } else {
        std::cout << "Destructeur appelé : Matrice partagée, compteur = " << *compteur << std::endl;
    }
}

// Accéder à un élément (lecture)
int Matrice::getElement(int i, int j) const {
    return tab[i * colonnes + j];
}

// Modifier un élément (écriture)
void Matrice::setElement(int i, int j, int valeur) {
    tab[i * colonnes + j] = valeur;
}

// Afficher la matrice
void Matrice::afficher() const {
    for (int i = 0; i < lignes; i++) {
        for (int j = 0; j < colonnes; j++) {
            std::cout << tab[i * colonnes + j] << " ";
        }
        std::cout << std::endl;
    }
}

// Multiplication de 2 matrices
Matrice Matrice::operator*(const Matrice& C) const {
    assert(colonnes == C.lignes && "Multiplication impossible : nombre de colonnes != nombre de lignes de l'autre matrice");
    
    Matrice D(lignes, C.colonnes);

    for (int i = 0; i < lignes; i++) {
        for (int j = 0; j < C.colonnes; j++) {
            int somme = 0;
            for (int k = 0; k < colonnes; k++) {
                somme += getElement(i, k) * C.getElement(k, j);
            }
            D.setElement(i, j, somme);
        }
    }

    return D;
}

// Remplir la matrice avec des valeurs aléatoires
void Matrice::remplirAleatoire(int min, int max) {
    for (int i = 0; i < lignes; i++) {
        for (int j = 0; j < colonnes; j++) {
            int valeurAleatoire = min + rand() % (max - min + 1);
            setElement(i, j, valeurAleatoire);
        }
    }
}
