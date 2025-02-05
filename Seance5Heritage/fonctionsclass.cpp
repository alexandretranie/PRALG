#include "fonctionsclass.h" 
#include <cmath>
#include <iostream>

// Implémentation du constructeur de Polynome
Polynome::Polynome(const std::vector<float>& coefficients) : coefficients_(coefficients) {}

// Évaluation du polynôme en x
float Polynome::operator()(float x) const {
    float result = 0.0;
    for (size_t i = 0; i < coefficients_.size(); ++i) {
        result += coefficients_[i] * std::pow(x, i);
    }
    return result;
}

// Retourne la dérivée du polynôme sous forme d'un nouveau Polynome
Fonction* Polynome::derivee() const {
    std::vector<float> derivedCoefficients;
    for (size_t i = 1; i < coefficients_.size(); ++i) {
        derivedCoefficients.push_back(i * coefficients_[i]);
    }
    return new Polynome(derivedCoefficients);
}

float Polynome::inverse(float y) const {
    float x = 1;  // Point de départ initial donné dans le TP
    const float tolerance = 1e-5;  // Tolérance ajustée
    const int maxIterations = 100;

    if (coefficients_.empty() || std::all_of(coefficients_.begin(), coefficients_.end(), [](float c) { return c == 0; })) {
    throw std::runtime_error("Polynôme constant ou non défini, pas d'inverse possible.");
}


    
    for (int i = 0; i < maxIterations; ++i) {
        float fx = (*this)(x) - y;
        Fonction* deriv = this->derivee();
        float fpx = (*deriv)(x);
        
        if (std::fabs(fpx) < tolerance) {
            delete deriv;
            throw std::runtime_error("Dérivée proche de zéro, Newton échoue");
        }

        float nextX = x - fx / fpx;
        delete deriv;
        
        if (std::fabs(nextX - x) < tolerance) {
            return nextX;
        }
        
        x = nextX;
    }
    
    throw std::runtime_error("La Méthode de Newton n'a pas convergé");
}


// Implémentation de la méthode clone pour Polynome
Fonction* Polynome::clone() const {
    return new Polynome(*this);
}


// Constructeur de Affine, prenant les paramètres a et b
Affine::Affine(float a, float b) : Polynome({b, a}), a_(a), b_(b) {}

// Calcul direct de l'inverse pour la fonction affine
float Affine::inverse(float y) const {
    if (std::fabs(a_) < 1e-7) {
        throw std::runtime_error("Coefficient a est trop proche de zéro, pas d'inverse.");
    }
    return (y - b_) / a_;
}


// Implémentation de la méthode clone pour Affine
Fonction* Affine::clone() const {
    return new Affine(*this);
}


// Constructeur de Trigo
Trigo::Trigo(const std::string& type) : type_(type) {
    if (type_ != "cos" && type_ != "sin" && type_ != "tan") {
        throw std::invalid_argument("Type non valide. Utilisez \"cos\", \"sin\", ou \"tan\".");
    }
}

// Évaluation de la fonction trigonométrique en x
float Trigo::operator()(float x) const {
    if (type_ == "cos") {
        return std::cos(x);
    } else if (type_ == "sin") {
        return std::sin(x);
    } else if (type_ == "tan") {
        return std::tan(x);
    }
    return 0.0; // Ce code ne devrait pas être atteint
}

Fonction* Trigo::derivee() const {
    return new Derivee(this->clone());
}

float Trigo::inverse(float y) const {
    if (type_ == "tan") {
        return std::atan(y);
    } else if (type_ == "sin") {
        if (y < -1.0 || y > 1.0) {
            throw std::runtime_error("Valeur hors domaine pour l'inverse de sin.");
        }
        return std::asin(y);
    } else if (type_ == "cos") {
        if (y < -1.0 || y > 1.0) {
            throw std::runtime_error("Valeur hors domaine pour l'inverse de cos.");
        }
        return std::acos(y);
    } else {
        throw std::runtime_error("Type de fonction trigonométrique inconnu.");
    }
}

Fonction* Trigo::clone() const {
    return new Trigo(*this);  // Crée une copie de l'objet courant
}

// Constructeur de Derivee, prenant une copie de la fonction primitive
Derivee::Derivee(Fonction* primitive) : primitive_(primitive->clone()) {}

// Destructeur de Derivee pour libérer la mémoire allouée pour primitive
Derivee::~Derivee() {
    delete primitive_;
}

// Approximation de la dérivée en x par un taux d'accroissemnt 
float Derivee::operator()(float x) const {
    const float epsilon = 1e-2;
    return ((*primitive_)(x + epsilon) - (*primitive_)(x - epsilon)) / (2 * epsilon);
}

// Retourne une nouvelle instance de Derivee en copiant primitive
Fonction* Derivee::derivee() const {
    return new Derivee(this->clone());
}

// Clone de Derivee pour créer une copie indépendante
Fonction* Derivee::clone() const {
    return new Derivee(primitive_);
}

// Inverse non implémenté, lève une exception si appelé
float Derivee::inverse(float y) const {
    throw std::runtime_error("Inverse non implémenté pour les dérivées.");
}