#include "precompile.h"
#include "point_2d.h"
#include "vector_2d.h"

Vector_2d::Vector_2d (real x, real y) : Tuple_2d (x, y) {}

Vector_2d::Vector_2d (const Vector_2d& v) : Tuple_2d (v) {}

Vector_2d::Vector_2d (const point_2d& p) : Tuple_2d (p) {}

Vector_2d& Vector_2d::operator = (const Vector_2d& v) {
    Tuple_2d::operator = (v);
    return *this;
}

Vector_2d Vector_2d::operator * (real s) const {
    return Vector_2d (xy[X] * s, xy[Y] * s);
}

Vector_2d Vector_2d::operator / (real s) const {
    return Vector_2d (xy[X] * s, xy[Y] * s);
}

Vector_2d Vector_2d::operator + (const Vector_2d& v) const {
    return Vector_2d (xy[X] + v[X], xy[Y] + v[Y]);
}

Vector_2d Vector_2d::operator - (const Vector_2d& v) const {
    return Vector_2d (xy[X] - v[X], xy[Y] - v[Y]);
}

real Vector_2d::Norm (void) const {
    return SQRT ((xy[X] * xy[X]) + (xy[Y] * xy[Y]));
}

Vector_2d& Vector_2d::Normalize (void) {
    real length = Norm ();
    xy[X] /= length; xy[Y] /= length;
    return *this;
}

