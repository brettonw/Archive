#include "precompile.h"
#include "arcball.h"
#include "quaternion_3d.h"
#include "view.h"

Arcball::Arcball (const point_2d &cent, real rad) {
    center = vector_2d (cent);
    radius = rad;
}

void	Arcball::click (const point_2d &pt) {
    start_vec = mapToSphere (pt - center);
    start_pt = point_2d (start_vec[X], start_vec[Y]) + center;
}

matrix_3d	Arcball::drag (const point_2d &pt) {
    end_vec = mapToSphere (pt - center);
    end_pt = point_2d (end_vec[X], end_vec[Y]) + center;
    return Quaternion (start_vec, end_vec);
}

void	Arcball::drawBackground (void) const {
    point_2d A = point_2d (-radius, -radius) + center;
    point_2d B = point_2d (radius, radius) + center;
    gView->drawCircle (A, B, 0x00555555, 0x00999999);
}

void	Arcball::drawForeground (void) const {
    drawArc (5);
    gView->drawCrossHair (end_pt, 0x000000ff);
    gView->drawCrossHair (start_pt, 0x00ffffff);
}

void	Arcball::computeArc (short segs, const vector_3d &a, const vector_3d &b) const {
    if (segs) {
        vector_3d bisector = (a + b).Normalize () * radius;
        computeArc (segs - 1, a, bisector);
        computeArc (segs - 1, bisector, b);
    } else
        gView->drawLineToPt (point_2d (b[X], b[Y]) + center, 0x00ff0000);
}

void	Arcball::drawArc (short numsegs) const {
    gView->moveToPt (start_pt);
    computeArc (numsegs, start_vec, end_vec);
}

vector_3d	Arcball::mapToSphere (const point_2d &pt) {
    real r = (pt[X] * pt[X]) + (pt[Y] * pt[Y]);
    real radsq = radius * radius;
    if (r > radsq) {
        real s = radius / SQRT (r);
        return vector_3d (pt[X] * s, pt[Y] * s, R (0.0));
    } else
        return vector_3d (pt[X], pt[Y], SQRT (radsq - r));
}
