#include	"split.h"


#ifndef	    _POLYGON_LIST_
#include    "polygon_list_3d.h"
#endif  //  _POLYGON_LIST_

#ifndef     _BSP_TREE_
#define     _BSP_TREE_

MAKE_PTR_TO (BspNode);

class	BspTree : public PtrToBspNode {

    public:
    void insert (PtrToPolygonList_3d& list, hclass keep, hclass cur);
    void push (const PtrToPolygon_3d& poly, PtrToPolygonList_3d& result, hclass keep, hclass cur);
    void push (PtrToPolygonList_3d& list, PtrToPolygonList_3d& result, hclass keep, hclass cur);
    void reduce (void);
    void draw (const point_3d& eye) const;
    bool rayIntersection (const ray& r, PtrToPolygon_3d& poly_hit, point_3d& ipt) const;
};

#endif  //  _BSP_TREE_
