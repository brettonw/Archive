#include "point_2d.h"
#include "vector_2d.h"
#include "vector_3d.h"
#include "matrix_3d.h"

#ifndef ARCBALL
#define ARCBALL

class	Arcball {
private:
    void computeArc (short, const vector_3d&, const vector_3d&) const;
    void drawArc (short numsegs) const;
    vector_3d mapToSphere (const point_2d &pt);

protected:
    vector_2d center;
    real radius;
    vector_3d start_vec;
    vector_3d end_vec;
    point_2d start_pt;
    point_2d end_pt;

public:
    Arcball (const point_2d&, real);
    virtual	void click (const point_2d&);
    virtual	matrix_3d drag (const point_2d&);
    virtual	void drawBackground (void) const;
    virtual	void drawForeground (void) const;
};

#endif //ARCBALL
