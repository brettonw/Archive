#include "tuple_2d.h"

#ifndef	VECTOR_2D
#define	VECTOR_2D

#ifndef	POINT_2D
class	point_2d;
#endif

class	Vector_2d : public Tuple_2d {
    protected:
    public:
    Vector_2d (void) {}
    Vector_2d (real x, real y);
    Vector_2d (const Vector_2d &v);
    Vector_2d (const point_2d &p);
    Vector_2d& operator = (const Vector_2d &v);
    Vector_2d operator * (real s) const;
    Vector_2d operator / (real s) const;
    Vector_2d operator + (const Vector_2d &v) const;
    Vector_2d operator - (const Vector_2d &v) const;
    real Norm (void) const;
    Vector_2d& Normalize (void);
};

#endif
