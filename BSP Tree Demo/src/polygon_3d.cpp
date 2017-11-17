#include "precompile.h"
#include "polygon_3d.h"

Polygon_3d::Polygon_3d (point_3d *buffer, uint cnt, int a, ...) {
    va_list pts;
    va_start (pts, cnt);
    count = cnt;
    points = new point_3d[count];
    for (uint i = 0; i < count; i++)
        points[i] = buffer[va_arg (pts, int)];
    plane.Define (normal (), points[0]);
    va_end (pts);
}

Polygon_3d::Polygon_3d (point_3d *buffer, uint cnt) {
    count = cnt;
    points = new point_3d[count];
    for (uint i = 0; i < count; i++)
        points[i] = buffer[i];
    plane.Define (normal (), points[0]);
}

Polygon_3d::~Polygon_3d (void) {
    delete[] points;
}

vector_3d	Polygon_3d::normal (void) const {
    vector_3d	sum (ZERO_VECTOR);
    for (uint i = 0, last = count - 1; i < count; last = i, i++) {
        point_3d	A = points[last],
            B = points[i];
        sum[X] += (A[Y] - B[Y]) * (A[Z] + B[Z]);
        sum[Y] += (A[Z] - B[Z]) * (A[X] + B[X]);
        sum[Z] += (A[X] - B[X]) * (A[Y] + B[Y]);
    }
    return sum.Normalize ();
}

bool	Polygon_3d::contains (const point_3d& pt) const {
    coord	x, y;
    switch (plane.MajorAxis ()) {
        case X:	x = Y; y = Z; break;
        case Y:	x = Z; y = X; break;
        case Z:	x = X; y = Y; break;
    }
    real	tx = pt[x], ty = pt[y];
    point_3d	*p1 = &points[count - 1], *p2 = points;
    int		yflag0 = ((*p1)[y] >= ty),
        inside = FALSE;
    for (int i = count; i--;) {
        int	yflag1 = ((*p2)[y] >= ty);
        if (yflag0 != yflag1) {
            int	xflag0 = ((*p1)[x] >= tx);
            if (xflag0 == ((*p2)[x] >= tx)) {
                if (xflag0)
                    inside = !inside;
            } else {
                if (((*p2)[x] - ((*p2)[y] - ty) * ((*p1)[x] - (*p2)[x]) / ((*p1)[y] - (*p2)[y])) >= tx)
                    inside = !inside;
            }
        }
        yflag0 = yflag1;
        p1 = p2;
        p2++;
    }
    return bool (inside);
}

void	Polygon_3d::invert (void) {
    point_3d	*pts = new point_3d[count];
    short	reverse = count - 1;
    for (uint i = 0; i < count; i++)
        pts[i] = points[reverse - i];
    delete[] points;
    points = pts;
    plane.Invert ();
}

real		Polygon_3d::rayIntersection (const ray& r) const {
    return	plane.rayIntersection (r);
}
