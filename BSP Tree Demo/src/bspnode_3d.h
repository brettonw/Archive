#ifndef     _BSP_TREE_
#include    "bsptree_3d.h"
#endif  //  _BSP_TREE_

#ifndef     _BSP_NODE_
#define     _BSP_NODE_

class	BspNode : public CountedObject {
    friend	class	BspTree;

    protected:
    plane_3d plane;
    PtrToPolygonList_3d on;
    BspTree in;
    BspTree out;

    BspNode (const PtrToPolygon_3d& poly);
    ~BspNode (void);

    public:
    void		insert (PtrToPolygonList_3d& list, hclass keep);
    void		push (const PtrToPolygon_3d& poly, PtrToPolygonList_3d& result, hclass keep);
    void		push (PtrToPolygonList_3d& list, PtrToPolygonList_3d& result, hclass keep);
    void		reduce (void);
    void		draw (const point_3d	&eye) const;
    bool		rayIntersection (const ray& r, PtrToPolygon_3d& poly_hit, point_3d& ipt) const;
    bool		planeSearch (const ray& r, PtrToPolygon_3d& poly_hit, point_3d& ipt) const;//	find the intersection of a ray and the plane_3d list
};

#endif  //  _BSP_NODE_
