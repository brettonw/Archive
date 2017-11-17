#ifndef     _POLYGON_
#include	"polygon_3d.h"
#endif  //  _POLYGON_

#include	"bound_3d.h"


#ifndef     _PTR_TO_H_
#include    "ptr_to.h"
#endif  //  _PTR_TO_H_

#ifndef	    _POLYGON_LIST_
#define	    _POLYGON_LIST_

MAKE_PTR_TO (PolygonList_3d);

class	PolygonList_3d : public CountedObject {
    protected:
    std::vector<PtrToPolygon_3d> polygons;
    bound_3d box;

    public:
    PolygonList_3d (void);
    ~PolygonList_3d (void);

    void addToList (const PtrToPolygon_3d& poly);
    void append (const PtrToPolygonList_3d& list);
    PtrToPolygon_3d pop (void);
    bool empty (void) const;
    void draw (void) const;
    const bound_3d& getBoundingBox (void) const;
};

inline const bound_3d& PolygonList_3d::getBoundingBox (void) const {
    return box;
}

#endif  //  _POLYGON_LIST_
