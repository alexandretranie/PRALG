#include "quadtree/quadtree.h"
#include "zipimage.h"
#include <Imagine/Graphics.h>
#include <iostream>
#include <string>

using namespace Imagine;
using namespace std;



int main(int argc, char **argv) {

    // Activer la protection des feuilles partagées contre la destruction
    QuadTree<bool>::protect_leaves_from_destruction = true;

    // Charger le fichier image spécifié en ligne de commande ou l'image par défaut
    string image_file = (argc > 1) ? argv[1] : default_image_file;
    unsigned char* greyImage;
    int width, height;

    cout << "Loading image: " << image_file << endl;
    if (!loadGreyImage(image_file, greyImage, width, height)) {
        cerr << "Error loading image file " << image_file << endl;
        return 1;
    }

    // Calcul de la taille en puissance de 2 pour chaque dimension
    int target_size = std::max(next_power_of_2(width), next_power_of_2(height));

    // Affichage des dimensions ajustées
    cout << "Image size: " << width << "x" << height << endl;
    cout << "Dimensions ajustées à la puissance de 2: " << target_size << "x" << target_size << endl;
    cout << "Number of pixels: " << width * height << endl;


    // Conversion en image booléenne
    ///bool* boolImage = new bool[width * height];
    ///for (int y = 0; y < height; ++y) {
        ///for (int x = 0; x < width; ++x) {
           ///boolImage[y * width + x] = (greyImage[y * width + x] < 128); // Seuil pour noir/blanc
        ///}
    ///}

    // Définir le seuil de compression pour la différence d'intensité
    int threshold = 50; 

    // Encodage de l'image dans un quadtree avec dimensions ajustées
    QuadTree<unsigned char>* root = encode_with_lossy_compression(greyImage, 0, 0, target_size, width, width, height, true, threshold);
    
    
    // Calcul de la taille compressée
    size_t compressedSize = calculateQuadTreeSize(root);
    std::cout << "Taille de l'image compressée : " << compressedSize << " octets" << std::endl;

    // Calculer la taille d'origine
    size_t originalSize = width * height; // Taille en octets pour une image 8 bits par pixel
    double compressionRatio = static_cast<double>(compressedSize) / originalSize;
    std::cout << "Taux de compression : " << compressionRatio << std::endl;

    // Décodage de l'image pour affichage
    unsigned char* decodedImage = new unsigned char[width * height];    
    decode_with_padding(root, 0, 0, target_size, width, height, decodedImage);

    // Affichage de l'image décodée
    Window window = openWindow(width, height);
    putGreyImage(0, 0, decodedImage, width, height);
    
    // Nettoyage de la mémoire
    delete[] greyImage;
    delete[] decodedImage;


    // Suppression du quadtree
    delete root;

    // Libérer explicitement les feuilles partagées
    if (blackLeaf) delete blackLeaf;
    if (whiteLeaf) delete whiteLeaf;

    Imagine::endGraphics(); // Ferme la fenêtre graphique
    return 0;
}