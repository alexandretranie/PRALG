#include "point2d.h"

ostream& operator<<(ostream &os, const Point2D& pt) {
    return os << "(" << pt.x << "," << pt.y << ")";
}


/* Opérateur pour comparer les coordonées */
bool operator<(const Point2D& a, const Point2D& b) {
    if (a.x != b.x) return a.x < b.x;
    return a.y < b.y;
}

/* Opérateur pour tester si les coordonées sont différentes */
bool Point2D::operator!=(const Point2D& other) const {
    return !(*this == other);
}

/* Opérateur pour tester si les coordonées sont égales */
bool Point2D::operator==(const Point2D& other) const {
    return this->x == other.x && this->y == other.y;
}

