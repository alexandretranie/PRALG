#pragma once
#include <string>
#include <vector>

class Eleve {
    private:
        std::string nom;  // Nom de l'élève
        int note;         // Note de l'élève

    public:
        // Constructeur par défaut
        Eleve();

        // Constructeur avec les paramètres
        Eleve(const std::string& nom, int note);

        // Destructeur
        ~Eleve();

        // Méthode pour obtenir le nom
        std::string getNom() const;

        // Méthode pour obtenir la note
        int getNote() const;

        // Méthode pour modifier le nom
        void setNom(const std::string& nouveauNom);

        // Méthode pour modifier la note
        void setNote(int nouvelleNote);

        // Méthode pour afficher les informations de l'élève
        void afficherInfo() const;
};

// Foncteur pour comparer les noms des élèves avec un compteur de comparaisons
struct CompareNom {
    int* ptr; // Pointeur vers un compteur d'opérations

    // Constructeur pour initialiser le pointeur
    CompareNom(int* compteur) : ptr(compteur) {}

    bool operator()(const Eleve& e1, const Eleve& e2) const {
        ++*ptr; // On incrémente le compteur à chaque comparaison
        return e1.getNom() < e2.getNom();
    }
};


// Foncteur pour comparer les notes des élèves avec un compteur de comparaisons
struct CompareNote {
    int* ptr; // Pointeur vers un compteur d'opérations

    // Constructeur pour initialiser le pointeur
    CompareNote(int* compteur) : ptr(compteur) {}

    bool operator()(const Eleve& e1, const Eleve& e2) const {
        ++*ptr; // Incrémenter le compteur à chaque comparaison
        return e1.getNote() > e2.getNote(); // Tri par note décroissante
    }
};


// Fonction pour générer un nom aléatoire
std::string genererNomAleatoire();


// Fonction pour générer une note aléatoire entre 0 et 20
int genererNoteAleatoire();


// Fonction pour remplir un vecteur d'élèves avec des données aléatoires
void remplirEleves(std::vector<Eleve>& eleves, int nombreEleves);
