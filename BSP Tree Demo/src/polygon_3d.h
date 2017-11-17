#ifndef     _PTR_TO_H_
#include    "ptr_to.h"
#endif  //  _PTR_TO_H_

#include "plane_3d.h"

#ifndef     _POLYGON_
#define     _POLYGON_

MAKE_PTR_TO (Polygon_3d);

class	Polygon_3d : public Object_3d {
    protected:
    plane_3d plane;
    uint count;
    point_3d* points;

    public:
    Polygon_3d (point_3d* buffer, uint count, int a, ...);
    Polygon_3d (point_3d* buffer, uint count);
    virtual	~Polygon_3d (void);
    uint getCount (void) const;
    const point_3d& getVertex (int i) const;
    const plane_3d& getPlane (void) const;
    vector_3d normal (void) const;
    bool contains (const point_3d& pt) const;
    void invert (void);
    virtual	real rayIntersection (const ray& r) const;
};

inline	uint	Polygon_3d::getCount (void) const {
    return count;
}

inline	const	point_3d& Polygon_3d::getVertex (int i) const {
    return points[i];
}

inline	const	plane_3d& Polygon_3d::getPlane (void) const {
    return plane;
}

#endif  //  _POLYGON_
