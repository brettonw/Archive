#include	"split.h"
#include	"listptr_3d.h"

#ifndef     _PTR_TO_H_
#include    "ptr_to.h"
#endif  //  _PTR_TO_H_

#ifndef BSPTREE
#define BSPTREE

MAKE_PTR_TO (BspNode);

class	BspTree : public PtrToBspNode {

    public:
    void		insert (listptr list, hclass keep, hclass cur);
    void		push (polyptr poly, listptr result, hclass keep, hclass cur);
    void		push (listptr list, listptr result, hclass keep, hclass cur);
    void		reduce (void);
    void		draw (const point_3d& eye) const;
    bool		rayIntersection (const ray& r, polyptr& poly_hit, point_3d& ipt) const;
};

extern BspTree* gWorld;

#endif //BSPTREE
