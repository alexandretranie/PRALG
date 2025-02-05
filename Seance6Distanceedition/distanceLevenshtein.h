#ifndef DISTANCELEVENSHTEIN_H
#define DISTANCELEVENSHTEIN_H

#include <string>
#include <unordered_map>

// Déclaration des fonctions
int levenshtein_recursive(const std::string& s, const std::string& t, int i, int j, std::unordered_map<int, int>* cache = nullptr, bool damerau = false);
int levenshtein(const std::string& s, const std::string& t, bool use_memoization = true, bool damerau = false);
int levenshtein_iterative(const std::string& s, const std::string& t,  bool damerau = false);
std::vector<std::vector<int>> levenshtein_matrix(const std::string& s, const std::string& t, bool damerau = false);
void print_levenshtein_path(const std::string& s, const std::string& t, const std::vector<std::vector<int>>& dp,  bool damerau = false);
void print_matrix(const std::vector<std::vector<int>>& dp, const std::string& s, const std::string& t);
int damerau_levenshtein_linear_mem(const std::string& s, const std::string& t);
void measure_execution_times(const std::string& s, const std::string& t);

#endif // DISTANCELEVENSHTEIN_H
