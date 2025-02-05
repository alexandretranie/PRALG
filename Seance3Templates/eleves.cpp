#include "eleves.h"
#include <iostream>
#include <string> 
#include <random>
#include <ctime>
#include <vector>



// Constructeur par défaut
Eleve::Eleve() : nom(""), note(0) {}

// Constructeur avec les paramètres
Eleve::Eleve(const std::string& nom, int note) : nom(nom), note(note) {}

// Destructeur
Eleve::~Eleve() {
}

// Méthode pour obtenir le nom
std::string Eleve::getNom() const {
    return nom;
}

// Méthode pour obtenir la note
int Eleve::getNote() const {
    return note;
}

// Méthode pour modifier le nom
void Eleve::setNom(const std::string& nouveauNom) {
    nom = nouveauNom;
}

// Méthode pour modifier la note
void Eleve::setNote(int nouvelleNote) {
    note = nouvelleNote;
}

// Méthode pour afficher les informations de l'élève
void Eleve::afficherInfo() const {
    std::cout << "Nom: " << nom << ", Note: " << note << std::endl;
}



// Fonction pour générer un nom aléatoire
std::string genererNomAleatoire() {
    static const char lettres[] = "abcdefghijklmnopqrstuvwxyz";
    std::string nom;

    // Générer une première lettre majuscule
    nom += 'A' + (rand() % 26);

    // Générer une longueur aléatoire pour le nom (entre 1 et 10 lettres supplémentaires)
    int longueurNom = 1 + (rand() % 10);

    // Ajouter des lettres minuscules
    for (int i = 0; i < longueurNom; ++i) {
        nom += lettres[rand() % 26];
    }

    return nom;
}



// Fonction pour générer une note aléatoire entre 0 et 20
int genererNoteAleatoire() {
    return rand() % 21;  // Note entre 0 et 20
}

// Fonction pour remplir un vecteur d'élèves avec des données aléatoires
void remplirEleves(std::vector<Eleve>& eleves, int nombreEleves) {
    for (int i = 0; i < nombreEleves; ++i) {
        std::string nomAleatoire = genererNomAleatoire();
        int noteAleatoire = genererNoteAleatoire();
        eleves.push_back(Eleve(nomAleatoire, noteAleatoire));
    }
}

