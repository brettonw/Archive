#include "precompile.h"
#include "utility.h"
#include "tuple_2d.h"

Tuple_2d::Tuple_2d (const Tuple_2d &t) {
    xy[X] = t[X]; xy[Y] = t[Y];
}

Tuple_2d::Tuple_2d (real x, real y) {
    xy[X] = x; xy[Y] = y;
}

void Tuple_2d::operator = (const Tuple_2d &t) {
    xy[X] = t[X]; xy[Y] = t[Y];
}

bool Tuple_2d::operator == (const Tuple_2d &t) const {
    return bool ((FABS (xy[X] - t[X]) < EPSILON) &&
        (FABS (xy[Y] - t[Y]) < EPSILON));
}

bool Tuple_2d::operator != (const Tuple_2d &t) const {
    return bool ((FABS (xy[X] - t[X]) > EPSILON) ||
        (FABS (xy[Y] - t[Y]) > EPSILON));
}

void Tuple_2d::operator () (real x, real y) {
    xy[X] = x; xy[Y] = y;
}

real Tuple_2d::operator | (const Tuple_2d &t) const {
    return  (xy[X] * t[X]) + (xy[Y] * t[Y]);
}

