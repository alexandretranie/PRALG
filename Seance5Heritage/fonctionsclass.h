#ifndef FONCTIONSCLASS_H
#define FONCTIONSCLASS_H

#include <vector>
#include <string>

class Fonction {
public:
    // Destructeur virtuel par défaut 
    virtual ~Fonction() = default;

    // Évalue la fonction en x - méthode virtuelle pure
    virtual float operator()(float x) const = 0;

    // Retourne la fonction dérivée - méthode virtuelle pure
    virtual Fonction* derivee() const = 0;

    // Calcule l'antécédent de y par la méthode de Newton
    virtual float inverse(float y) const = 0;

    virtual Fonction* clone() const = 0;
};


class Polynome : public Fonction {
public:

    Polynome(const std::vector<float>& coefficients);

    float operator()(float x) const override;

    Fonction* derivee() const override;

    float inverse(float y) const override;

    Fonction* clone() const override;
private:
    std::vector<float> coefficients_;
};



class Affine : public Polynome {
public:

    Affine(float a, float b);

    float inverse(float y) const override;

    Fonction* clone() const override;
private:
    float a_, b_;
};



class Trigo : public Fonction {
public:
    Trigo(const std::string& type);
    float operator()(float x) const override;
    Fonction* derivee() const override;
    float inverse(float y) const override;
    Fonction* clone() const override;
private:
    std::string type_;
};


class Derivee : public Fonction {
public:
    // Constructeur prenant une copie de la fonction primitive
    explicit Derivee(Fonction* primitive);

    // Destructeur pour libérer la copie de primitive
    ~Derivee() override;

    // Approxime la dérivée en x par différences finies
    float operator()(float x) const override;

    // Retourne une nouvelle Derivee en copiant l'objet
    Fonction* derivee() const override;

    // Clone de l'objet Derivee
    Fonction* clone() const override;

    // Inverse non implémenté ici
    float inverse(float y) const override;

private:
    Fonction* primitive_;  // Pointeur vers la copie de la fonction primitive
};



#endif // FONCTIONSCLASS_H
