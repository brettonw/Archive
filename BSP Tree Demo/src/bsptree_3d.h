#include	"split.h"
#include	"listptr_3d.h"

#ifndef     _COUNTED_OBJECT_H_
#include    "counted_object.h"
#endif  //  _COUNTED_OBJECT_H_

#ifndef BSPTREE
#define BSPTREE

class	BspNode;
class	BspTree : public CountedObject {
protected:
    BspNode	*node;

public:
    BspTree (void);
    ~BspTree (void);
    void		insert (listptr list, hclass keep, hclass cur);
    void		push (polyptr poly, listptr result, hclass keep, hclass cur);
    void		push (listptr list, listptr result, hclass keep, hclass cur);
    void		reduce (void);
    void		draw (const point_3d	&eye) const;
    bool		rayIntersection (const ray &r, polyptr &poly_hit, point_3d &ipt) const;
};

extern BspTree* gWorld;

#endif //BSPTREE
