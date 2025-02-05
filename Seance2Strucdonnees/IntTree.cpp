#include "IntTree.h"
#include <iostream>
#include <string>  

// Constructeur 
IntTree::IntTree(int d): data(d) {}

// Destructeur 
IntTree::~IntTree() {
    for (IntTree* child: children){
        delete child;
    }
}

// Return information of this node
int IntTree::getData() const {
    return data;
}
    
// Set information of this node
void IntTree::setData(int d){
    data = d;
}
    
// Return the number of children of this node
int IntTree::nbChildren() const{
    return children.size();
};
    
// Return the child at pos, if any (left-most child at position 0), sinon renvoie l'erreur out of range
IntTree* IntTree::getChild(int pos){
    if (pos >= 0 && pos < children.size()){
        return children[pos];
    }
    else{
        throw std::out_of_range("Index out of range");
    }
}
    
// Replace the exisiting child at pos (left-most child at position 0) 
void IntTree::setChild(int pos, IntTree* newChild){
    if (pos >= 0 && pos < children.size()){
        children[pos]=newChild;
    }
    if(newChild == nullptr){
        throw std::invalid_argument("newChild cannot be nullptr");
    }
    else{
        throw std::out_of_range("Index out of range");
    }
}

    
// Add newChild as supplementary right-most child of this node
void IntTree::addAsLastChild(IntTree* newChild){
    if(newChild == nullptr){
        throw std::invalid_argument("newChild cannot be nullptr");
    }
    children.push_back(newChild);
}
    
// Remove right-most child of this node
bool IntTree::removeLastChild() {
    if (children.empty()) {
        return false;
    }
    delete children.back(); 
    children.pop_back();
    return true; 
}

// Display l'arbre
void IntTree::display(std::string prefix,std::string indent) const {
    std::cout << prefix << data << std::endl;
    for (const IntTree* child : children){
        if (child != nullptr){
            child->display(prefix + indent, indent);
        }
    }
}


// Calcul de la profondeur maximale de l'arbre sans allocation de mémoire
int IntTree::maxDepth() const {
    if (children.empty()) {
        // Si le nœud n'a pas d'enfants
        return 1;
    }

    // Variable pour suivre la profondeur maximale des enfants
    int max_child_depth = 0;
    
    // Parcourir chaque enfant et calculer la profondeur maximale
    for (const IntTree* child : children) {
        // Appel récursif
        int child_depth = child->maxDepth();
        if (child_depth > max_child_depth) {
            max_child_depth = child_depth;
        }
    }
    return 1 + max_child_depth;
}


// Méthode pour calculer la profondeur minimale de l'arbre
int IntTree::minDepth() const {
    if (children.empty()) {
        // Si le nœud n'a pas d'enfants, c'est une feuille, donc profondeur = 1
        return 1;
    }

    // Variable pour suivre la profondeur minimale des enfants, initialisée avec une valeur élevée
    int min_child_depth = std::numeric_limits<int>::max();

    // Parcourir chaque enfant et calculer la profondeur minimale
    for (const IntTree* child : children) {
        // Appel récursif, mais sans créer de variables temporaires ou allouer de mémoire
        int child_depth = child -> minDepth();
        if (child_depth < min_child_depth) {
            min_child_depth = child_depth;
        }
    }

    // La profondeur actuelle est 1 + la profondeur minimale trouvée parmi les enfants
    return 1 + min_child_depth;
}
