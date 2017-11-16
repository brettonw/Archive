#include "coord.h"

#ifndef TUPLE_2D
#define TUPLE_2D

class Tuple_2d {
    protected:
    real xy[2];
    public:
    Tuple_2d (void) {}
    Tuple_2d (const Tuple_2d& t);
    Tuple_2d (real x, real y);
    void operator = (const Tuple_2d& t);
    bool operator == (const Tuple_2d& t) const;
    bool operator != (const Tuple_2d& t) const;
    void operator () (real x, real y);
    real operator | (const Tuple_2d& t) const;
    real operator [] (coord c) const;
    real& operator [] (coord c);
};

inline real Tuple_2d::operator [] (coord c) const {
    return xy[c];
}

inline real& Tuple_2d::operator [] (coord c) {
    return xy[c];
}

#endif
