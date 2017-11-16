#include "bsptree_3d.h"

#ifndef BSPNODE
#define BSPNODE

class	BspNode {
    friend	class	BspTree;
private:
protected:
    plane_3d		plane;
    listptr	on;
    BspTree	in;
    BspTree	out;
    BspNode (const polyptr &poly);
    ~BspNode (void);

public:
    void		insert (listptr list, hclass keep);
    void		push (polyptr poly, listptr result, hclass keep);
    void		push (listptr list, listptr result, hclass keep);
    void		reduce (void);
    void		draw (const point_3d	&eye) const;
    bool		rayIntersection (const ray &r, polyptr &poly_hit, point_3d &ipt) const;
    bool		planeSearch (const ray &r, polyptr &poly_hit, point_3d &ipt) const;//	find the intersection of a ray and the plane_3d list
};

#endif //BSPNODE
