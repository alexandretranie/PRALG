#pragma once


class Matrice {
private:
    int* tab;
    int* compteur;  // Compteur de partage
    int lignes;
    int colonnes;

public:
    // Constructeur
    Matrice(int l, int c);

    // Constructeur par copie
    Matrice(const Matrice& autre);

    // Destructeur
    ~Matrice();

    // Accéder à un élément (lecture)
    int getElement(int i, int j) const;

    // Modifier un élément (écriture)
    void setElement(int i, int j, int valeur);

    // Afficher la matrice
    void afficher() const;

    // Multiplication de 2 matrices 
    Matrice operator*(const Matrice& C) const;

    void remplirAleatoire(int min, int max);

};