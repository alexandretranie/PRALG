#include <iostream>
#include "fonctionsclass.h"

int main() {
    try {
        // Exemple 1 : Calcul de l'inverse pour le polynôme x -> x^3
        std::vector<float> coefficients = {0, 0, 0, 1}; // Correspond au polynôme x^3
        Polynome cube(coefficients);
        float y = 27.0;
        float inverseCube = cube.inverse(y);
        std::cout << "L'inverse de la fonction x -> x^3 évalué en " << y << " est : " << inverseCube << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Erreur (x^3) : " << e.what() << std::endl;
    }

    try {
        // Test avec un polynôme nul
        std::vector<float> zeroCoefficients = {0, 0, 0, 0}; // Polynôme nul
        Polynome zeroPolynome(zeroCoefficients);
        zeroPolynome.inverse(1); // Devrait lever une exception
    } catch (const std::exception& e) {
        std::cerr << "Test polynôme nul : " << e.what() << std::endl;
    }

    try {
        // Test avec un polynôme valide (x^2)
        std::vector<float> validCoefficients = {0, 0, 1}; // Correspond à x^2
        Polynome square(validCoefficients);
        float inverseSquare = square.inverse(4); // Devrait retourner 2
        std::cout << "L'inverse de x^2 pour y=4 est : " << inverseSquare << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Test polynôme x^2 : " << e.what() << std::endl;
    }

    try {
        // Exemple 2 : Calcul de l'inverse pour la fonction trigonométrique tan(x)
        Trigo tangent("tan");
        float piEstimate = tangent.inverse(1.0) * 4;
        std::cout << "Estimation de pi en utilisant 4 * atan(1) : " << piEstimate << std::endl;

        // Calcul de la dérivée première de tan
        Fonction* tanDerivative = tangent.derivee();
        float pi_over_4 = M_PI / 4;
        float firstDerivative = (*tanDerivative)(pi_over_4);
        std::cout << "tan'(" << pi_over_4 << ") = " << firstDerivative << std::endl; // Devrait approximer 2

        // Calcul de la dérivée seconde de tan
        Fonction* tanSecondDerivative = tanDerivative->derivee();
        float secondDerivative = (*tanSecondDerivative)(pi_over_4);
        std::cout << "tan''(" << pi_over_4 << ") = " << secondDerivative << std::endl; // Devrait approximer 4

        // Calcul direct de la formule analytique pour la dérivée seconde de tan
        float tanValue = std::tan(pi_over_4);
        float analyticalSecondDerivative = 2 * tanValue * (1 + tanValue * tanValue);
        std::cout << "Valeur analytique de tan''(" << pi_over_4 << ") = " << analyticalSecondDerivative << std::endl;

        // Libérer la mémoire allouée
        delete tanDerivative;
        delete tanSecondDerivative;

    } catch (const std::exception& e) {
        std::cerr << "Erreur (tan) : " << e.what() << std::endl;
    }

    return 0;
}
