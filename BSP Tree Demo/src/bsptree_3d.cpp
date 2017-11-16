#include "precompile.h"
#include "bspnode_3d.h"

BspTree* gWorld;

BspTree::BspTree (void) {
    node = 0;
}

BspTree::~BspTree (void) {
    if (node)
        delete node;
}

void BspTree::insert (listptr list, hclass keep, hclass cur) {
    if (list->Empty ()) return;
    if (node)
        node->insert (list, keep);
    else
        if ((cur == keep) || (keep == HC_SPANNING)) {
            node = new BspNode (list->Pop ());
            if (!list->Empty ())
                node->insert (list, HC_SPANNING);
        }
}

void BspTree::push (polyptr poly, listptr result, hclass keep, hclass cur) {
    if (node)
        node->push (poly, result, keep);
    else
        if (cur == keep)
            result->AddToList (poly);
}

void BspTree::push (listptr list, listptr result, hclass keep, hclass cur) {
    if (list->Empty ()) return;
    if (node)
        node->push (list, result, keep);
    else
        if (cur == keep)
            result->Append (list);
}

void BspTree::reduce (void) {
    if (node)
        node->reduce ();
}

void BspTree::draw (const point_3d &eye) const {
    if (node)
        node->draw (eye);
}

bool BspTree::rayIntersection (const ray &r, polyptr &poly_hit, point_3d &ipt) const {
    if (node)
        return node->rayIntersection (r, poly_hit, ipt);
    return FALSE;
}

