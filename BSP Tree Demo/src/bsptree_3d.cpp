#include "precompile.h"
#include "bspnode_3d.h"

BspTree* gWorld;

void BspTree::insert (listptr list, hclass keep, hclass cur) {
    if (list->Empty ()) return;
    if (ptr) {
        ptr->insert (list, keep);
    } else if ((cur == keep) or (keep == HC_SPANNING)) {
        ptr = new BspNode (list->Pop ());
        if (!list->Empty ()) {
            ptr->insert (list, HC_SPANNING);
        }
    }
}

void BspTree::push (polyptr poly, listptr result, hclass keep, hclass cur) {
    if (ptr) {
        ptr->push (poly, result, keep);
    } else if (cur == keep) {
        result->AddToList (poly);
    }
}

void BspTree::push (listptr list, listptr result, hclass keep, hclass cur) {
    if (list->Empty ()) return;
    if (ptr) {
        ptr->push (list, result, keep);
    } else if (cur == keep) {
        result->Append (list);
    }
}

void BspTree::reduce (void) {
    if (ptr) {
        ptr->reduce ();
    }
}

void BspTree::draw (const point_3d &eye) const {
    if (ptr) {
        ptr->draw (eye);
    }
}

bool BspTree::rayIntersection (const ray &r, polyptr &poly_hit, point_3d &ipt) const {
    if (ptr) {
        return ptr->rayIntersection (r, poly_hit, ipt);
    }
    return FALSE;
}

