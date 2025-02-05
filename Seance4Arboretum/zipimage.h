// zipimage.h
#ifndef ZIPIMAGE_H
#define ZIPIMAGE_H

#include "quadtree/quadtree.h"

extern const char* default_image_file;
extern QuadLeaf<bool>* blackLeaf;
extern QuadLeaf<bool>* whiteLeaf;

QuadTree<bool>* encodeToQuadTree(const bool* image, int x, int y, int size, int imgWidth, bool useQuadDAG);
void decodeQuadTree(const QuadTree<bool>* node, bool* image, int x, int y, int size, int imgWidth);
void displayDecodedImage(const QuadTree<bool>* root, int imgSize);


void drawQuadrants(const QuadTree<bool>* node, int x, int y, int size);

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
int next_power_of_2(int n);
bool is_out_of_bounds(int x, int y, int orig_width, int orig_height);
QuadTree<bool>* encode_with_padding(const bool* image, int x, int y, int size, int imgWidth, int orig_width, int orig_height, bool useQuadDAG);
void decode_with_padding(QuadTree<unsigned char>* node, int x, int y, int size, int orig_width, int orig_height, unsigned char* img);
QuadTree<unsigned char>* encode_with_lossy_compression(const unsigned char* image, int x, int y, int size, int imgWidth, int orig_width, int orig_height, bool useQuadDAG, int threshold);


#endif