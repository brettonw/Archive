#include "precompile.h"
#include "bspnode_3d.h"

void BspTree::insert (PtrToPolygonList_3d& list, hclass keep, hclass cur) {
    if (list->empty ()) return;
    if (ptrTo) {
        ptrTo->insert (list, keep);
    } else if ((cur == keep) or (keep == HC_SPANNING)) {
        setPtr (new BspNode (list->pop ()));
        // add ref

        if (!list->empty ()) {
            ptrTo->insert (list, HC_SPANNING);
        }
    }
}

void BspTree::push (const PtrToPolygon_3d& poly, PtrToPolygonList_3d& result, hclass keep, hclass cur) {
    if (ptrTo) {
        ptrTo->push (poly, result, keep);
    } else if (cur == keep) {
        result->addToList (poly);
    }
}

void BspTree::push (PtrToPolygonList_3d& list, PtrToPolygonList_3d& result, hclass keep, hclass cur) {
    if (list->empty ()) return;
    if (ptrTo) {
        ptrTo->push (list, result, keep);
    } else if (cur == keep) {
        result->append (list);
    }
}

void BspTree::reduce (void) {
    if (ptrTo) {
        ptrTo->reduce ();
    }
}

void BspTree::draw (const point_3d& eye) const {
    if (ptrTo) {
        ptrTo->draw (eye);
    }
}

bool BspTree::rayIntersection (const ray & r, PtrToPolygon_3d& poly_hit, point_3d& ipt) const {
    if (ptrTo) {
        return ptrTo->rayIntersection (r, poly_hit, ipt);
    }
    return FALSE;
}

