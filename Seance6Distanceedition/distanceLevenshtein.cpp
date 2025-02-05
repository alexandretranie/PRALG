#include "distanceLevenshtein.h"
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <climits>
#include <ctime>


// Fonction récursive pour calculer la distance de Levenshtein
int levenshtein_recursive(const std::string& s, const std::string& t, int i, int j, std::unordered_map<int, int>* cache, bool damerau) {
    const int MAX_SIZE = s.size() + t.size();
    int key = i * MAX_SIZE + j;

    // Mémoïsation si besoin 
    if (cache && cache->count(key)) {
        return (*cache)[key];
    }

    // Cas de base
    if (i == 0) return j;
    if (j == 0) return i;

    // Substitution
    int cost = (s[i - 1] == t[j - 1]) ? 0 : 1;
    int result = std::min({
        levenshtein_recursive(s, t, i - 1, j, cache, damerau) + 1,  // Suppression
        levenshtein_recursive(s, t, i, j - 1, cache, damerau) + 1,  // Insertion
        levenshtein_recursive(s, t, i - 1, j - 1, cache, damerau) + cost // Substitution
    });

    // Transpositions
    if (damerau && i > 1 && j > 1 && s[i - 1] == t[j - 2] && s[i - 2] == t[j - 1]) {
        result = std::min(result, levenshtein_recursive(s, t, i - 2, j - 2, cache, damerau) + cost);
    }

    // Mémoïsation si besoin
    if (cache) {
        (*cache)[key] = result;
    }

    return result;
}


// Fonction d'assistance pour faciliter l'appel
int levenshtein(const std::string& s, const std::string& t, bool use_memoization, bool damerau) {
    if (use_memoization) {
        std::unordered_map<int, int> cache;
        return levenshtein_recursive(s, t, s.size(), t.size(), &cache, damerau);
    } else {
        return levenshtein_iterative(s, t, damerau);
    }
}



int levenshtein_iterative(const std::string& s, const std::string& t, bool damerau) {
    int m = s.size(), n = t.size();
    std::vector<int> prev(n + 1), curr(n + 1);

    // Initialisation de la première ligne
    for (int j = 0; j <= n; ++j) prev[j] = j;

    // Remplissage itératif
    for (int i = 1; i <= m; ++i) {
        curr[0] = i;
        for (int j = 1; j <= n; ++j) {
            int cost = (s[i - 1] == t[j - 1]) ? 0 : 1;

            // Calcul de la distance minimale
            curr[j] = std::min({prev[j] + 1,          // Suppression
                                curr[j - 1] + 1,      // Insertion
                                prev[j - 1] + cost}); // Substitution

            // Gestion des transpositions
            if (damerau && i > 1 && j > 1 && s[i - 1] == t[j - 2] && s[i - 2] == t[j - 1]) {
                curr[j] = std::min(curr[j], prev[j - 2] + cost);
            }
        }
        std::swap(prev, curr); // Passer à la ligne suivante
    }

    return prev[n];
}


// Fonction pour calculer la matrice des distances
std::vector<std::vector<int>> levenshtein_matrix(const std::string& s, const std::string& t, bool damerau) {
    int m = s.size(), n = t.size();
    std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1));

    // Initialisation des bords
    for (int i = 0; i <= m; ++i) dp[i][0] = i;
    for (int j = 0; j <= n; ++j) dp[0][j] = j;

    // Remplissage de la matrice
    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {
            int cost = (s[i - 1] == t[j - 1]) ? 0 : 1;
            dp[i][j] = std::min({dp[i - 1][j] + 1,       // Suppression
                                 dp[i][j - 1] + 1,       // Insertion
                                 dp[i - 1][j - 1] + cost}); // Substitution

            // Gestion des transpositions (Damerau-Levenshtein)
            if (damerau && i > 1 && j > 1 &&
                s[i - 1] == t[j - 2] && s[i - 2] == t[j - 1]) {
                dp[i][j] = std::min(dp[i][j], dp[i - 2][j - 2] + cost); // Transposition
            }
        }
    }
    return dp;
}



void print_levenshtein_path(const std::string& s, const std::string& t, const std::vector<std::vector<int>>& dp, bool damerau) {
    int i = s.size();
    int j = t.size();
    std::string current = s;

    std::cout << "Départ: " << current << std::endl;

    while (i > 0 || j > 0) {
        // Priorité : Transposition (Damerau-Levenshtein)
        if (damerau && i > 1 && j > 1 &&
            s[i - 1] == t[j - 2] && s[i - 2] == t[j - 1] &&
            dp[i][j] == dp[i - 2][j - 2] + 1) {
            std::cout << "Transposition de '" << current[i - 2] << "' et '" << current[i - 1] << "': ";
            std::swap(current[i - 2], current[i - 1]);
            std::cout << current << std::endl;
            i -= 2;
            j -= 2;
        }
        // Suppression
        else if (i > 0 && dp[i][j] == dp[i - 1][j] + 1) {
            std::cout << "Suppression de '" << current[i - 1] << "': ";
            current.erase(i - 1, 1);
            std::cout << current << std::endl;
            --i;
        }
        // Insertion
        else if (j > 0 && dp[i][j] == dp[i][j - 1] + 1) {
            std::cout << "Insertion de '" << t[j - 1] << "' à la position " << i << ": ";
            current.insert(i, 1, t[j - 1]);
            std::cout << current << std::endl;
            --j;
        }
        // Substitution
        else if (i > 0 && j > 0 && dp[i][j] == dp[i - 1][j - 1] + (s[i - 1] != t[j - 1])) {
            if (s[i - 1] != t[j - 1]) {
                std::cout << "Substitution de '" << current[i - 1] << "' en '" << t[j - 1] << "': ";
                current[i - 1] = t[j - 1];
                std::cout << current << std::endl;
            }
            --i;
            --j;
        }
        // Aucun changement nécessaire (diagonale)
        else {
            --i;
            --j;
        }
    }
}

// Affichage de la matrice pour suivre les transformationss
void print_matrix(const std::vector<std::vector<int>>& dp, const std::string& s, const std::string& t) {
    std::cout << "  ";
    for (char c : t) std::cout << "  " << c;
    std::cout << std::endl;
    for (size_t i = 0; i <= s.size(); ++i) {
        if (i > 0) std::cout << s[i - 1] << " ";
        else std::cout << "  ";
        for (size_t j = 0; j <= t.size(); ++j) {
            std::cout << dp[i][j] << "  ";
        }
        std::cout << std::endl;
    }
}


int damerau_levenshtein_linear_mem(const std::string& s, const std::string& t) {
    int m = s.size(), n = t.size();
    if (m < n) return damerau_levenshtein_linear_mem(t, s); // Assure que m >= n pour minimiser l'espace utilisé.

    std::vector<int> prev(n + 1), curr(n + 1), prev2(n + 1); // Ligne actuelle, précédente, et avant-précédente.

    // Initialisation de la première ligne
    for (int j = 0; j <= n; ++j) prev[j] = j;

    for (int i = 1; i <= m; ++i) {
        curr[0] = i; // Initialisation de la première colonne pour la ligne courante
        for (int j = 1; j <= n; ++j) {
            int cost = (s[i - 1] == t[j - 1]) ? 0 : 1;

            // Calculer le coût minimal
            curr[j] = std::min({prev[j] + 1,          // Suppression
                                curr[j - 1] + 1,      // Insertion
                                prev[j - 1] + cost}); // Substitution

            // Gestion des transpositions
            if (i > 1 && j > 1 && s[i - 1] == t[j - 2] && s[i - 2] == t[j - 1]) {
                curr[j] = std::min(curr[j], prev2[j - 2] + cost); // Transposition
            }
        }

        // Faire avancer les lignes
        prev2 = prev;
        prev = curr;
    }

    return curr[n];
}


void measure_execution_times(const std::string& s, const std::string& t) {

    std::cout << "         " << std::endl;
    clock_t t1, t2;

    // Récursif sans mémoïsation
    t1 = clock();
    int result_recursive = levenshtein_recursive(s, t, s.size(), t.size(), nullptr, false);
    t2 = clock();
    std::cout << "Temps (Récursif sans mémoïsation) : " << (t2 - t1) / (float)CLOCKS_PER_SEC << "s" << std::endl;

    // Récursif avec mémoïsation
    t1 = clock();
    int result_memo = levenshtein_recursive(s, t, s.size(), t.size(), new std::unordered_map<int, int>(), false);
    t2 = clock();
    std::cout << "Temps (Récursif avec mémoïsation) : " << (t2 - t1) / (float)CLOCKS_PER_SEC << "s" << std::endl;

    // Programmation dynamique itérative
    t1 = clock();
    int result_iterative = levenshtein_iterative(s, t, false);
    t2 = clock();
    std::cout << "Temps (Itératif) : " << (t2 - t1) / (float)CLOCKS_PER_SEC << "s" << std::endl;

    // Damerau-Levenshtein (Optimisé en espace)
    t1 = clock();
    int result_damerau = damerau_levenshtein_linear_mem(s, t);
    t2 = clock();
    std::cout << "Temps (Damerau-Levenshtein, linéaire en espace) : " << (t2 - t1) / (float)CLOCKS_PER_SEC << "s" << std::endl;

}
