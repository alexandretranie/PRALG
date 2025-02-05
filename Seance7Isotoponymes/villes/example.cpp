#include <iostream>
#include <string>
#include <vector>
#include <time.h>
#include <cmath>
#include <limits>
#include <fstream>
#include <map>
#include <set>
#include <sstream>
#include "town.h"
#include "point2d.h"

using namespace std;


// The path and name of the town file
string townFile = srcPath "/villes.txt";




/*
 * Example of use of "town.h" and "villes.txt":
 * - read file of metropolitan French towns with coordinates
 * - display reading time and number of towns read
 * - ask for town name
 * - lookup for town coordinates with this name and display them
 * - display distance between two random towns
 */



void afficherHistogramme(const vector<string>& noms) {
    // Table associative pour compter les occurrences des noms de villes
    map<string, int> comptage;
    for (const auto& nom : noms) {
        comptage[nom]++;
    }

    // Table associative pour construire l'histogramme
    map<int, int> histogramme;
    for (const auto& [nom, count] : comptage) {
        histogramme[count]++;
    }

    // Afficher l'histogramme
    cout << "Histogramme des répétitions de noms de villes :" << endl;
    for (const auto& [repetitions, nombre] : histogramme) {
        cout << nombre << " noms de villes sont utilisés par " << repetitions << " ville(s) exactement." << endl;
    }
}



void afficherHistogrammeParCoordonnees(const vector<Town>& towns) {
    // Comptage des occurrences de chaque coordonnée
    map<Point2D, int> coord_count;
    for (const auto& town : towns) {
        coord_count[town.point()]++;
    }

    // Construire l'histogramme
    map<int, int> histogram;
    for (const auto& [coord, count] : coord_count) {
        histogram[count]++;
    }

    // Affichage de l'histogramme
    cout << "Histogramme des villes partageant les mêmes coordonnées :" << endl;
    for (const auto& [nb_villes, nb_coord] : histogram) {
        cout << nb_coord << " coordonnées sont partagées par " << nb_villes << " ville(s)." << endl;
    }
}

void testArtificialDataset() {
    // Création d'un jeu de données artificiel
    vector<Town> towns = {
        Town("A", 0, 0),
        Town("B", 1, 1),
        Town("A", 1, 1),
        Town("B", 0, 0)
    };

    // Afficher les villes du jeu de données artificiel
    cout << "Test sur un dataset artificiel :" << endl;
    for (const auto& town : towns) {
        cout << "Town " << town.name() << " at (" << town.x() << ", " << town.y() << ")" << endl;
    }

    // Calculer les cas de confusion
    int ambiguous_count = calculateAmbiguousPairs(towns);

    cout << "Nombre de cas ambigus dans un dataset artificiel : " << ambiguous_count << endl;
}





int main() {
    /////
    ///// Read town file
    /////
    vector<Town> towns; // Create an empty list of towns
    float xmin = std::numeric_limits<float>::max();
    float ymin = std::numeric_limits<float>::max();
    float xmax = std::numeric_limits<float>::min();
    float ymax = std::numeric_limits<float>::min();

    // Read file
    cout << "Reading town file: " << townFile << endl;
    clock_t t1 = clock();
    int nb_towns = Town::read_file(townFile, towns, xmin, ymin, xmax, ymax);
    clock_t t2 = clock();

    // Check for errors
    if (nb_towns < 0) {
        cout << "No town found" << endl;
        return 1;
    }

    cout << "File read in " << ((float)(t2 - t1) / CLOCKS_PER_SEC) << " s" << endl;
    cout << "Number of towns in file: " << nb_towns << endl;
    cout << "Bounding box: (" << xmin << "," << ymin << ")-(" << xmax << "," << ymax << ")" << endl;

    // Afficher l'histogramme des répétitions de noms
    vector<string> noms;
    for (const Town& town : towns) {
        noms.push_back(town.name());
    }
    afficherHistogramme(noms);

    afficherHistogrammeParCoordonnees(towns);

    //Calcul de N, C, et leur intersection    
    std::set<Town> N = calculateN(towns); // Villes avec des noms partagés 
    std::set<Town> C = calculateC(towns); // Villes avec des coordonnées partagées
    std::set<Town> NC = intersectionNC(N, C); // Intersection de N et C

    cout << "|N| (villes avec des noms partagés): " << N.size() << endl;
    cout << "|C| (villes avec des coordonnées partagées): " << C.size() << endl;
    cout << "|N ∩ C| (intersection): " << NC.size() << endl;

    string name;
    cout << "What town are you interested in? ";
    getline(cin, name); // Lecture du nom de ville 

    vector<Town> ntowns = towns_with_name(towns, name); // Cherche la ville par noms
    cout << "Number of towns with this name: " << ntowns.size() << endl;
    for (const auto& town : ntowns) {
        cout << "  (lat=" << town.lat() << ", lon=" << town.lon() << ")" << endl;
    }

    // Tire deux villes aléatoires et calcule leur distance
    srand(clock());
    Town town1 = towns[rand() % towns.size()];
    Town town2 = towns[rand() % towns.size()];
    cout << "By the way, did you know that " << town1.name()
         << " was " << town1.dist(town2)
         << " km away from " << town2.name() << " ?" << endl;

    // Appel de la fonction pour calculer les paires ambiguës
    int ambiguous_count = calculateAmbiguousPairs(towns);

    // Test avec des données artificielles
    testArtificialDataset();

    // Comparer les temps de calcul
    compareCalculationTimes(towns);

    return 0;
}