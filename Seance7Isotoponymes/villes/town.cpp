#include <iostream>
#include <fstream>
#include <cmath>
#include <limits>
#include <set>
#include <map>
#include "town.h"
using namespace std;

pair<float,float> geoToLambert93(float latitude,float longitude);

// Construct a new town object given its name, latitude and longitude
Town::Town(string name, float lat, float lon)
: _name(name), _lat(lat), _lon(lon), _coords(0,0) {
    pair<float,float> coords = geoToLambert93(lat,lon);
    _coords.x = coords.first;
    _coords.y = coords.second;
}

// Construct a new dummy town
Town::Town() : _name(""), _lat(0), _lon(0), _coords(0,0) {}

// Return the distance to given town (in kilometers)
float Town::dist(const Town& t) const {
    return sqrt((x()-t.x())*(x()-t.x())+(y()-t.y())*(y()-t.y()));
}

// Read file of town names and coordinates, and add them into a vector
// Return number of towns read, or negative value if some error occurred
int Town::read_file(string town_file, vector<Town>& towns,
                    float& xmin, float& ymin, float& xmax, float& ymax) {
    // Open file stream
    ifstream townfs(town_file.c_str());
    // Check opening was OK
    if (!townfs.is_open()) {
        cerr << "Cannot open file: " << town_file << endl;
        return -1; // File not found, permission denied...
    }
    // Read file
    int nb_towns = 0;
    while (true) {
        // Read name (stopping at delimiting character tab)
        string name;
        getline(townfs, name, '\t');
        if (!townfs.good()) { // Break on error or end of file
            if (name.empty()) townfs.clear(); // No error if end of file
            else townfs >> name; // (Dummy) error is line not empty
            break;
        }
        // Read coordinates
        float latitude;
        float longitude;
        townfs >> latitude >> longitude;
        if (townfs.fail()) break; // Break l'erreur
        // Flush line
        string dummy;
        getline(townfs, dummy);
        if (townfs.fail()) break; // Break l'erreur
        // Remember town
        Town town(name,latitude,longitude);
        towns.push_back(town);
        nb_towns++;
        xmin = min(town.x(),xmin);
        ymin = min(town.y(),ymin);
        xmax = max(town.x(),xmax);
        ymax = max(town.y(),ymax);
    }
    // In case an error occurred
    if (townfs.fail()) { 
        cerr << "Error while reading file " << town_file
             << " at line: " << (nb_towns+1) << endl;
        return -2; // Bad line format
    }
    // Return the number of towns read
    return nb_towns;
}

int Town::read_file(string town_file, vector<Town>& towns) {
    float xmin = std::numeric_limits<float>::max();
    float ymin = std::numeric_limits<float>::max();
    float xmax = std::numeric_limits<float>::min();
    float ymax = std::numeric_limits<float>::min();
    return Town::read_file(town_file, towns, xmin, ymin, xmax, ymax);
}

vector<Town> Town::read_towns(const string& townFile) {
    vector<Town> towns;
    cout << "Reading town file: " << townFile << endl;
    clock_t time1 = clock();
    int nb_towns = Town::read_file(townFile, towns);
    clock_t time2 = clock();
    if (nb_towns < 0)
        cerr << "Error while reading file" << endl;
    else if (nb_towns == 0)
        cerr << "No town found" << endl;
    else {
        srand(time(NULL));
        cout << "File read in: "
             << ((float)(time2-time1)/CLOCKS_PER_SEC) << " s" << endl;
        cout << "Number of towns in file: "
             << towns.size() /* same as nb_towns */ << endl;
        cout << "A random town, using format \"name[lat,lon](x,y)\": "
             << towns[rand() % towns.size()] << endl;
    }
    return towns;
}

// Return a vector of towns with given name
vector<Town> towns_with_name(const vector<Town>& t, string name) {
    vector<Town> found;
    for (vector<Town>::const_iterator it=t.begin(); it!=t.end(); ++it)
        if(it->name() == name)
            found.push_back(*it);
    return found;
}

// Return a vector of towns with given Lambert 93 (x,y) coordinates
vector<Town> towns_with_x_y(const vector<Town>& t, float x, float y) {
    vector<Town> found;
    for (vector<Town>::const_iterator it=t.begin(); it!=t.end(); ++it)
        if(it->x() == x && it->y() == y)
            found.push_back(*it);
    return found;
}

// Return a vector of towns with given geographic coordinates
// (latitude, longitude)
vector<Town> towns_with_lat_lon(const vector<Town>& t, float lat, float lon) {
    vector<Town> found;
    for (vector<Town>::const_iterator it=t.begin(); it!=t.end(); ++it)
        if(it->lat() == lat && it->lon() == lon)
            found.push_back(*it);
    return found;
}

//
// Display town: name[lat,lon](x,y)
//
ostream& operator<<(ostream &os, const Town& town) {
    return os << town.name()
              << "[" << town.lat() << "," << town.lon() << "]"
              << town.point();
}

/*
 * Lambert93 projection utilities
 */

#include <cmath>
using std::cos;
using std::exp;
using std::log;
using std::pow;
using std::sin;
using std::sqrt;
using std::tan;
#include <utility>
using std::make_pair;

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

//
// Constants used for the Lambert 93 projection
//
inline float degreeToRadian(float deg){ return deg/180*M_PI; }
const float a = 6378137;       // semi-major axis of the ellipsoid
const float e = 0.08181919106; // first eccentricity of the ellipsoid
const float lc = degreeToRadian(3.f);
const float l0 = degreeToRadian(3.f);
const float phi1 = degreeToRadian(44.f); // 1st automecoic parallel
const float phi2 = degreeToRadian(49.f); // 2nd automecoic parallel
const float phi0 = degreeToRadian(46.5f);// latitude of origin
const float X0 =  700000; // x coordinate at origin
const float Y0 = 6600000; // y coordinate at origin
// Normals
const float gN1 = a/sqrt(1-e*e*sin(phi1)*sin(phi1));
const float gN2 = a/sqrt(1-e*e*sin(phi2)*sin(phi2));
// Isometric latitudes
const float gl1=log(tan(M_PI/4+phi1/2)*pow((1-e*sin(phi1))/(1+e*sin(phi1)),e/2));
const float gl2=log(tan(M_PI/4+phi2/2)*pow((1-e*sin(phi2))/(1+e*sin(phi2)),e/2));
const float gl0=log(tan(M_PI/4+phi0/2)*pow((1-e*sin(phi0))/(1+e*sin(phi0)),e/2));
// Projection exponent
const float n = (log((gN2*cos(phi2))/(gN1*cos(phi1))))/(gl1-gl2);
// Projection constant
const float c = ((gN1*cos(phi1))/n)*exp(n*gl1);
// Coordinate
const float ys = Y0 + c*exp(-n*gl0);

// Convert geographic coordinates (latitude, longitude in degrees) into
// cartesian coordinates (in kilometers) using the Lambert 93 projection.
pair<float,float> geoToLambert93(float latitude,float longitude) {
    float phi = degreeToRadian(latitude);
    float l   = degreeToRadian(longitude);
    float gl  = log(tan(M_PI/4+phi/2)*pow((1-e*sin(phi))/(1+e*sin(phi)),e/2));
    float x93 = X0 + c*exp(-n*gl)*sin(n*(l-lc));
    float y93 = ys - c*exp(-n*gl)*cos(n*(l-lc));
    return make_pair(x93/1000,y93/1000);
}


bool operator<(const Town& a, const Town& b) {
    if (a.x() != b.x()) return a.x() < b.x();
    if (a.y() != b.y()) return a.y() < b.y();
    return a.name() < b.name();
}


// Fonction pour calculer l'ensemble N 
set<Town> calculateN(const vector<Town>& towns) {
    map<string, vector<Town>> name_map;
    for (const Town& town : towns) {
        name_map[town.name()].push_back(town);
    }

    set<Town> N;
    for (const auto& [name, town_list] : name_map) {
        if (town_list.size() > 1) {
            N.insert(town_list.begin(), town_list.end());
        }
    }
    return N;
}


// Fonction pour calculer l'ensemble C
set<Town> calculateC(const vector<Town>& towns) {
    map<Point2D, vector<Town>> coord_map;
    for (const Town& town : towns) {
        coord_map[town.point()].push_back(town);
    }

    set<Town> C;
    for (const auto& [coord, town_list] : coord_map) {
        if (town_list.size() > 1) {
            C.insert(town_list.begin(), town_list.end());
        }
    }
    return C;
}


// Fonction pour faire l'intersection 
set<Town> intersectionNC(const set<Town>& N, const set<Town>& C) {
    set<Town> intersection;
    std::set_intersection(N.begin(), N.end(), C.begin(), C.end(),
                          std::inserter(intersection, intersection.begin()));
    return intersection;
}



// Fonction pour calculer les villes satisfaisant la condition
set<vector<string>> calculateConfusionCases(const vector<Town>& towns) {

    // Groupement par coordonnées et par noms
    map<Point2D, vector<Town>> coords_map;
    map<string, vector<Town>> names_map;

    for (const Town& town : towns) {
        coords_map[town.point()].push_back(town);
        names_map[town.name()].push_back(town);
    }

    set<vector<string>> unique_cases; 

    // Parcours pour trouver les cas de confusion
    for (const Town& v1 : towns) {
        const auto& same_coords = coords_map[v1.point()];
        const auto& same_name = names_map[v1.name()];

        // Parcourir v2 avec mêmes coordonnées que v1
        for (const Town& v2 : same_coords) {
            if (v1.name() == v2.name()) continue; // Ignore si nom identique (pas valide)

            // Parcourir v3 avec même nom que v1
            for (const Town& v3 : same_name) {
                if (v1.point() == v3.point()) continue; // Ignore si coordonnées identiques (pas valide)

                // Vérifier v4 : même nom que v2, mêmes coordonnées que v3
                const auto& v4_candidates = coords_map[v3.point()];
                for (const Town& v4 : v4_candidates) {
                    if (v4.name() == v2.name()) {

                        // Ajouter une solution unique
                        vector<string> case_key = {v1.name(), v2.name(), v3.name(), v4.name()};
                        sort(case_key.begin(), case_key.end()); 
                        unique_cases.insert(case_key);
                    }
                }
            }
        }
    }

    return unique_cases;
}


// Fonction optimisée pour calculer les pairs ambigus
int calculateAmbiguousPairs(const vector<Town>& towns) {
    cout << "Calcul des paires de villes ambigus" << endl;

    // Groupe les villes par coordonnées et noms
    map<Point2D, vector<Town>> coords_map;
    map<string, vector<Town>> names_map;

    for (const Town& town : towns) {
        coords_map[town.point()].push_back(town);
        names_map[town.name()].push_back(town);
    }

    set<vector<string>> unique_cases; 

    for (const Town& v1 : towns) {
        // Villes avec les mêmes coordonées que v1
        const auto& same_coords = coords_map[v1.point()];
        // Villes avec le même nom que v1
        const auto& same_name = names_map[v1.name()];

        for (const Town& v2 : same_coords) {
            if (v1.name() == v2.name()) continue;

            for (const Town& v3 : same_name) {
                if (v1.point() == v3.point()) continue;

                // Vérifie pour v4 : même coordonnées que v3 et même nom que v2
                const auto& v4_candidates = coords_map[v3.point()];
                for (const Town& v4 : v4_candidates) {
                    if (v4.name() == v2.name()) {
                        // Créer une clef unique 
                        vector<string> case_key = {v1.name(), v2.name(), v3.name(), v4.name()};
                        sort(case_key.begin(), case_key.end());
                        unique_cases.insert(case_key);
                    }
                }
            }
        }
    }

    cout << "Nombre de cas ambigus: " << unique_cases.size() << endl;
    return unique_cases.size();
}



// Calculer les pairs ambigus de façon naïve
int calculateAmbiguousPairsNaive(const vector<Town>& towns) {
    int ambiguous_count = 0;

    for (const Town& v1 : towns) {
        for (const Town& v2 : towns) {
            if (v1.point() == v2.point() && v1.name() != v2.name()) {
                for (const Town& v3 : towns) {
                    if (v1.name() == v3.name() && v1.point() != v3.point()) {
                        for (const Town& v4 : towns) {
                            if (v3.point() == v4.point() && v2.name() == v4.name()) {
                                ambiguous_count++;
                            }
                        }
                    }
                }
            }
        }
    }

    // Divise par 4 la somme par symétrie
    ambiguous_count /= 4;

    return ambiguous_count;
}


void compareCalculationTimes(const vector<Town>& towns) {
    cout << "Comparer l'approche naïve et optimisée..." << endl;

    // Temps pour l'approche naïve
    clock_t start_naive = clock();
    int ambiguous_count_naive = calculateAmbiguousPairsNaive(towns);
    clock_t end_naive = clock();
    double time_naive = static_cast<double>(end_naive - start_naive) / CLOCKS_PER_SEC;

    cout << "Approche naïve trouvée " << ambiguous_count_naive << " cas ambigus en "
         << time_naive << " secondes." << endl;

    // Temps pour l'approche optimisée
    clock_t start_optimized = clock();
    int ambiguous_count_optimized = calculateAmbiguousPairs(towns);
    clock_t end_optimized = clock();
    double time_optimized = static_cast<double>(end_optimized - start_optimized) / CLOCKS_PER_SEC;

    cout << "Approche optimisée trouvée " << ambiguous_count_optimized << " cas ambigus en "
         << time_optimized << " secondes." << endl;

    // Comparaison des temps
    double speedup = time_naive / time_optimized;
    cout << "Gain de temps de : " << speedup << "x" << endl;
}
