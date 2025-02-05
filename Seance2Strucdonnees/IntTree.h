#pragma once
#include <vector>
#include <string>

// Node of a tree containing an integer at each node
class IntTree {
    // Node information
    int data;
    // Sequence of children (empty if none)
    std::vector<IntTree*> children;
public:
    // Create a node with given information
    IntTree(int d);

    // Destruct a node and all its descendents
    ~IntTree();

    // Return information of this node
    int getData() const;

    // Set information of this node
    void setData(int d);

    // Return the number of children of this node
    int nbChildren() const;

    // Return the child at pos, if any (left-most child at position 0)
    /**
     * Retourne l'enfant à la position donnée.
     * 
     * Erreur :
     * - Si 'pos' est hors des limites (inférieur à 0 ou supérieur au nombre d'enfants),
     *   cette fonction lance une exception std::out_of_range.
     * 
     * @param pos : index de l'enfant (premier enfant à l'index 0)
     * @return un pointeur vers l'enfant si 'pos' est valide.
     */
    IntTree* getChild(int pos);

    // Replace the exisiting child at pos (left-most child at position 0) 
    /**
     * Remplace l'enfant existant à la position donnée.
     * 
     * Erreurs :
     * - Si 'pos' est hors des limites, la fonction lance une exception std::out_of_range.
     * - Si 'newChild' est nul (nullptr), elle lance une exception std::invalid_argument.
     * 
     * @param pos : position où remplacer l'enfant
     * @param newChild : pointeur vers le nouvel enfant
     */
    void setChild(int pos, IntTree* newChild);

    // Add newChild as supplementary right-most child of this node
    /**
     * Ajoute 'newChild' en tant qu'enfant supplémentaire à la fin.
     * 
     * Erreur :
     * - Si 'newChild' est nul (nullptr), cette fonction lance une exception std::invalid_argument.
     * 
     * @param newChild : pointeur vers le nouvel enfant à ajouter
     */
    void addAsLastChild(IntTree* newChild);

    // Remove right-most child of this node
    /**
     * Supprime l'enfant le plus à droite de ce nœud.
     * 
     * Erreur :
     * - Si ce nœud n'a pas d'enfants, la fonction ne fait rien et retourne 'false'.
     * 
     * @return booléen indiquant si la suppression a été effectuée
     */
    bool removeLastChild();

    // Méthode récursive pour parcourir un arbre en profondeur
    /**
     * Affiche les valeurs du nœud et de ses enfants en profondeur avec un préfixe et une indentation.
     * 
     * Erreur :
     * - Aucun signalement d'erreur particulier ici, car il s'agit simplement d'affichage.
     * 
     * @param prefix : chaîne de caractères affichée avant chaque nœud
     * @param indent : chaîne de caractères utilisée pour indenter les enfants
     */
    void display(std::string prefix = "", std::string indent = "  ") const;


    // Calcul de la profondeur maximale de l'arbre sans allocation de mémoire
    int maxDepth() const;

    // Calcul de la profondeur minimale de l'arbre sans allocation de mémoire
    int minDepth() const;
};