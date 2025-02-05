#include <Imagine/Graphics.h>
#include <iostream>
#include "quadtree/quadtree.h"
#include <cmath>
#include <algorithm>


using namespace Imagine;
using namespace std;

// The path and name of the town file
const char* default_image_file=srcPath("Lena.png");


// Feuilles globales partagées pour les pixels noirs et blancs
QuadLeaf<bool>* blackLeaf = nullptr;
QuadLeaf<bool>* whiteLeaf = nullptr;




QuadTree<bool>* encodeToQuadTree(const bool* image, int x, int y, int size, int imgWidth, bool useQuadDAG) {
    // Initialisation des feuilles noires et blanches partagées
    if (useQuadDAG) {
        if (blackLeaf == nullptr) blackLeaf = new QuadLeaf<bool>(false);
        if (whiteLeaf == nullptr) whiteLeaf = new QuadLeaf<bool>(true);
    }

    if (size == 1) {
        bool color = image[y * imgWidth + x];
        // Utilisation des feuilles partagées si le QuadDAG est activé
        if (useQuadDAG) {
            return color ? whiteLeaf : blackLeaf;
        } else {
            return new QuadLeaf<bool>(color);
        }
    }

    int halfSize = size / 2;
    QuadTree<bool>* topLeft = encodeToQuadTree(image, x, y, halfSize, imgWidth, useQuadDAG);
    QuadTree<bool>* topRight = encodeToQuadTree(image, x + halfSize, y, halfSize, imgWidth, useQuadDAG);
    QuadTree<bool>* bottomLeft = encodeToQuadTree(image, x, y + halfSize, halfSize, imgWidth, useQuadDAG);
    QuadTree<bool>* bottomRight = encodeToQuadTree(image, x + halfSize, y + halfSize, halfSize, imgWidth, useQuadDAG);

    if (topLeft->isLeaf() && topRight->isLeaf() && bottomLeft->isLeaf() && bottomRight->isLeaf()) {
        bool color = topLeft->value();
        if (color == topRight->value() && color == bottomLeft->value() && color == bottomRight->value()) {
            if (!useQuadDAG) {
                delete topLeft;
                delete topRight;
                delete bottomLeft;
                delete bottomRight;
                return new QuadLeaf<bool>(color);
            }
            // Réutiliser les feuilles partagées si elles ont la même couleur
            return color ? whiteLeaf : blackLeaf;
        }
    }

    return new QuadNode<bool>(topLeft, topRight, bottomLeft, bottomRight);
}

// Fonction récursive pour décoder le quadtree en une image
void decodeQuadTree(const QuadTree<bool>* node, bool* image, int x, int y, int size, int imgWidth) {
    if (node->isLeaf()) {
        // Remplissage de la région avec la couleur de la feuille
        bool color = node->value();
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                image[(y + i) * imgWidth + (x + j)] = color;
            }
        }
    } else {
        // Récursion sur les sous-régions
        int halfSize = size / 2;
        decodeQuadTree(node->child(0), image, x, y, halfSize, imgWidth); // Top-left
        decodeQuadTree(node->child(1), image, x + halfSize, y, halfSize, imgWidth); // Top-right
        decodeQuadTree(node->child(2), image, x, y + halfSize, halfSize, imgWidth); // Bottom-left
        decodeQuadTree(node->child(3), image, x + halfSize, y + halfSize, halfSize, imgWidth); // Bottom-right
    }
}

// Fonction pour dessiner les contours des quadrants
void drawQuadrants(const QuadTree<bool>* node, int x, int y, int size) {
    if (node->isLeaf()) {
        return; // Pas besoin de dessiner les contours pour une feuille
    }

    // Dessiner le contour pour le bloc courant
    Imagine::drawRect(x, y, size, size, RED);

    // Récursivement, dessiner les contours des sous-blocs
    int halfSize = size / 2;
    drawQuadrants(node->child(0), x, y, halfSize); // Top-left
    drawQuadrants(node->child(1), x + halfSize, y, halfSize); // Top-right
    drawQuadrants(node->child(2), x, y + halfSize, halfSize); // Bottom-left
    drawQuadrants(node->child(3), x + halfSize, y + halfSize, halfSize); // Bottom-right
}

// Fonction principale pour décoder le quadtree en une image
void displayDecodedImage(const QuadTree<bool>* root, int imgSize) {
    // Création de l'image décodée
    bool* image = new bool[imgSize * imgSize];

    // Décodage du quadtree en remplissant l'image
    decodeQuadTree(root, image, 0, 0, imgSize, imgSize);

    // Affichage de l'image décodée
    Imagine::openWindow(imgSize, imgSize);
    for (int y = 0; y < imgSize; ++y) {
        for (int x = 0; x < imgSize; ++x) {
            Color color = image[y * imgSize + x] ? BLACK : WHITE;
            Imagine::drawPoint(x, y, color);
        }
    }

    drawQuadrants(root, 0, 0, imgSize);

    delete[] image;
}

// Fonction récursive pour calculer la taille du quadtree
template<typename T>
size_t calculateQuadTreeSize(const QuadTree<T>* node) {
    if (node->isLeaf()) {
        return sizeof(QuadLeaf<T>); // Taille d'une feuille
    } else {
        size_t size = sizeof(QuadNode<T>); // Taille d'un nœud
        for (int i = 0; i < 4; i++) {
            size += calculateQuadTreeSize(node->child(i)); // Ajouter la taille des sous-arbres
        }
        return size;
    }
}


int next_power_of_2(int n) {
    if (n <= 0) return 1; // Gestion des cas non positifs
    return std::pow(2, std::ceil(std::log2(n)));
}

bool is_out_of_bounds(int x, int y, int orig_width, int orig_height) {
    // Vérifie si (x, y) est hors des limites de l'image originale
    return x >= orig_width || y >= orig_height;
}


// Pour une image qui n'est pas nécessairement carré 
QuadTree<bool>* encode_with_padding(const bool* image, int x, int y, int size, int imgWidth, int orig_width, int orig_height, bool useQuadDAG) {
    if (size == 1) {
        // Cas de base : un seul pixel
        if (is_out_of_bounds(x, y, orig_width, orig_height)) {
            return new QuadLeaf<bool>(true); // Feuille blanche par défaut
        }
        return new QuadLeaf<bool>(image[y * imgWidth + x]);
    }

    int halfSize = size / 2;

    // Appels récursifs pour diviser l'image en quatre sous-régions
    QuadTree<bool>* topLeft = encode_with_padding(image, x, y, halfSize, imgWidth, orig_width, orig_height, useQuadDAG);
    QuadTree<bool>* topRight = encode_with_padding(image, x + halfSize, y, halfSize, imgWidth, orig_width, orig_height, useQuadDAG);
    QuadTree<bool>* bottomLeft = encode_with_padding(image, x, y + halfSize, halfSize, imgWidth, orig_width, orig_height, useQuadDAG);
    QuadTree<bool>* bottomRight = encode_with_padding(image, x + halfSize, y + halfSize, halfSize, imgWidth, orig_width, orig_height, useQuadDAG);

    // Vérifie si les quatre sous-arbres peuvent être fusionnés en une seule feuille
    if (topLeft->isLeaf() && topRight->isLeaf() && bottomLeft->isLeaf() && bottomRight->isLeaf()) {
        bool color = topLeft->value();
        if (topRight->value() == color && bottomLeft->value() == color && bottomRight->value() == color) {
            // Si les quatre quadrants sont identiques, on les fusionne en une seule feuille
            delete topLeft;
            delete topRight;
            delete bottomLeft;
            delete bottomRight;
            return new QuadLeaf<bool>(color);
        }
    }

    // Créer un nœud avec les quatre sous-arbres
    return new QuadNode<bool>(topLeft, topRight, bottomRight, bottomLeft);
}


void decode_with_padding(QuadTree<unsigned char>* node, int x, int y, int size, int orig_width, int orig_height, unsigned char* img) {
    if (node->isLeaf()) {
        unsigned char color = node->value();  // Récupérer la valeur d'intensité en niveaux de gris
        for (int i = 0; i < size && x + i < orig_width; ++i) {
            for (int j = 0; j < size && y + j < orig_height; ++j) {
                img[(y + j) * orig_width + (x + i)] = color;
            }
        }
        return;
    }

    int halfSize = size / 2;
    decode_with_padding(node->child(NW), x, y, halfSize, orig_width, orig_height, img);
    decode_with_padding(node->child(NE), x + halfSize, y, halfSize, orig_width, orig_height, img);
    decode_with_padding(node->child(SW), x, y + halfSize, halfSize, orig_width, orig_height, img);
    decode_with_padding(node->child(SE), x + halfSize, y + halfSize, halfSize, orig_width, orig_height, img);
}


// Pour une image en noir et blanc
QuadTree<byte>* encode_with_lossy_compression(const byte* image, int x, int y, int size, int imgWidth, int orig_width, int orig_height, bool useQuadDAG, int threshold) {
    if (size == 1) {
        // Si la taille est de 1, on retourne une feuille avec la valeur d'intensité
        if (is_out_of_bounds(x, y, orig_width, orig_height)) {
            return new QuadLeaf<byte>(255); // Par défaut, blanc
        }
        return new QuadLeaf<byte>(image[y * imgWidth + x]);
    }

    int halfSize = size / 2;
    QuadTree<byte>* topLeft = encode_with_lossy_compression(image, x, y, halfSize, imgWidth, orig_width, orig_height, useQuadDAG, threshold);
    QuadTree<byte>* topRight = encode_with_lossy_compression(image, x + halfSize, y, halfSize, imgWidth, orig_width, orig_height, useQuadDAG, threshold);
    QuadTree<byte>* bottomLeft = encode_with_lossy_compression(image, x, y + halfSize, halfSize, imgWidth, orig_width, orig_height, useQuadDAG, threshold);
    QuadTree<byte>* bottomRight = encode_with_lossy_compression(image, x + halfSize, y + halfSize, halfSize, imgWidth, orig_width, orig_height, useQuadDAG, threshold);

    // Vérifier si les quatre quadrants sont des feuilles
    if (topLeft->isLeaf() && topRight->isLeaf() && bottomLeft->isLeaf() && bottomRight->isLeaf()) {
        // Calculer les intensités des quatre feuilles
        byte tl = topLeft->value();
        byte tr = topRight->value();
        byte bl = bottomLeft->value();
        byte br = bottomRight->value();

        // Calculer l'intensité maximale et minimale
        byte maxIntensity = std::max({tl, tr, bl, br});
        byte minIntensity = std::min({tl, tr, bl, br});

        // Si la différence est inférieure au seuil, on fusionne les feuilles
        if (maxIntensity - minIntensity < threshold) {
            // Calculer l'intensité moyenne arrondie
            byte averageIntensity = static_cast<byte>((tl + tr + bl + br) / 4);

            // Supprimer les anciens sous-arbres
            delete topLeft;
            delete topRight;
            delete bottomLeft;
            delete bottomRight;

            return new QuadLeaf<byte>(averageIntensity);
        }
    }

    // Créer un nœud avec les quatre sous-arbres si on ne peut pas les fusionner
    return new QuadNode<byte>(topLeft, topRight, bottomRight, bottomLeft);
}