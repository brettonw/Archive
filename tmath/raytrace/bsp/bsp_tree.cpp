//-----------------------------------------------------------------------------
// Copyright (C) 2003 Microsoft, All rights reserved
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// includes
//-----------------------------------------------------------------------------
#include    "tgeom.h"
#include    "intersect.h"

//-----------------------------------------------------------------------------
// types
//-----------------------------------------------------------------------------
typedef TVector2<double>    Vector;
typedef TPoint2<double>     Point;
typedef TEdge2<double>      Edge;
class   EdgeList : public std::list<Edge> {};

//-----------------------------------------------------------------------------
// types
//-----------------------------------------------------------------------------
typedef sInt    HClass;
enum
{
    HC_INSIDE = TURNS_LEFT, 
    HC_COINCIDENT = COLINEAR, 
    HC_OUTSIDE = TURNS_RIGHT,
    HC_SPANNING = SPANNING,
    HC_KEEP_ALL = 2,
    HC_DONT_CARE = 3
};

//-----------------------------------------------------------------------------
// definitions of node and tree. The tree itself is basically an abstraction
// on a pointer to a node, and the node is what does all the work.
//-----------------------------------------------------------------------------
class BSP_Tree
{
    friend class BSP_Tree_Node;
    
    public:
        /* void */      BSP_Tree (void);
        /* void */      ~BSP_Tree (void);
        void            Insert (EdgeList& edgeList, HClass keep = HC_KEEP_ALL, HClass current = HC_OUTSIDE);
        void            Reduce (void);
        void            Enumerate (void);

    private:
        void            Push (const EdgeList& edgeList, EdgeList& result, HClass keep, HClass current);
        
    private:
        BSP_Tree_Node*  m_pNode;
};

//-----------------------------------------------------------------------------
class BSP_Tree_Node
{
    friend class BSP_Tree;
    private:
        /* void */      BSP_Tree_Node (EdgeList& edgeList);
        /* void */      ~BSP_Tree_Node (void);
        void            Insert (const EdgeList& edgeList, HClass keep);
        void            Reduce (void);
        void            Enumerate (void);

    private:
        void            Push (const EdgeList& edgeList, EdgeList& result, HClass keep);
        void            Push (const EdgeList& edgeList, EdgeList& inside, EdgeList& coincident, EdgeList& outside);

    private:
        BSP_Tree        m_in;
        BSP_Tree        m_out;
        Edge            m_splitEdge;
        EdgeList        m_coincidentEdgeList;
};

//-----------------------------------------------------------------------------
// class method definitions
//-----------------------------------------------------------------------------
/* void */  
BSP_Tree::BSP_Tree (void)
{
    m_pNode = 0;
}

//-----------------------------------------------------------------------------
/* void */  
BSP_Tree::~BSP_Tree (void)
{
    if (m_pNode)
        delete m_pNode;
}

//-----------------------------------------------------------------------------
void  
BSP_Tree::Insert (EdgeList& edgeList, HClass keep, HClass current)
{
    // check to see there is a sub-tree rooted here. If not, we'll need to
    // create one.
    if (not m_pNode)
    {
        // before we try to create a new sub-tree, we want to know if we
        // really want to keep these bits. If not, clear out the edgelist
        if ((keep == HC_KEEP_ALL) or (current == keep))
        {
            m_pNode = new BSP_Tree_Node (edgeList);
            m_pNode->Insert (edgeList, HC_KEEP_ALL);
        }
    }
    else
        m_pNode->Insert (edgeList, keep);
}

//-----------------------------------------------------------------------------
void  
BSP_Tree::Reduce (void)
{
    if (m_pNode)
        m_pNode->Reduce ();
}

//-----------------------------------------------------------------------------
void  
BSP_Tree::Enumerate (void)
{
    if (m_pNode)
        m_pNode->Enumerate ();
}

//-----------------------------------------------------------------------------
void  
BSP_Tree::Push (const EdgeList& edgeList, EdgeList& result, HClass keep, HClass current)
{
    if (edgeList.size () > 0)
    {
        if (m_pNode)
            m_pNode->Push (edgeList, result, keep);
        else if (current == keep)
        {
            EdgeList::const_iterator    iter = edgeList.begin ();
            EdgeList::const_iterator    iterEnd = edgeList.end ();
            while (iter != iterEnd)
                result.push_back (*iter++);
        }
    }
}

//-----------------------------------------------------------------------------
/* void */  
BSP_Tree_Node::BSP_Tree_Node (EdgeList& edgeList)
{
    // choose an edge to divide space with, store it in the node, and remove 
    // it from the edge list. For now, we use the end of the list for speed,
    // but better results will probably be had by choosing a random edge
    m_splitEdge = edgeList.back ();
    m_coincidentEdgeList.push_back (m_splitEdge);
    edgeList.pop_back ();
}

//-----------------------------------------------------------------------------
/* void */  
BSP_Tree_Node::~BSP_Tree_Node (void)
{
}

//-----------------------------------------------------------------------------
void  
BSP_Tree_Node::Insert (const EdgeList& edgeList, HClass keep)
{
    // push the edge list across the splitting edge for this node
    EdgeList    inside;
    EdgeList    outside;
    Push (edgeList, inside, m_coincidentEdgeList, outside);
    
    // insert the split edge list in the two sub-trees
    if (inside.size () > 0)
        m_in.Insert (inside, keep, HC_INSIDE);
    if (outside.size () > 0)
        m_out.Insert (outside, keep, HC_OUTSIDE);
}

//-----------------------------------------------------------------------------
void  
BSP_Tree_Node::Reduce (void)
{
    // push the coincident edge list down through the in node, then push the 
    // results through the out sub-tree keeping the bits that land outside
    EdgeList    boundary;
    m_out.Push (m_coincidentEdgeList, boundary, HC_OUTSIDE, HC_OUTSIDE);
    
    // store the result back to the coincident list
    m_coincidentEdgeList.assign (boundary.begin (), boundary.end ());
    
    // reduce the sub-trees
    m_in.Reduce ();
    m_out.Reduce ();
}

//-----------------------------------------------------------------------------
void  
BSP_Tree_Node::Enumerate (void)
{
    EdgeList::const_iterator    iter = m_coincidentEdgeList.begin ();
    EdgeList::const_iterator    iterEnd = m_coincidentEdgeList.end ();
    while (iter != iterEnd)
    {
        const Edge& edge = *iter++;
        DebugPrint ("Edge: (%f, %f) to (%f, %f)\n", edge.a.x, edge.a.y, edge.b.x, edge.b.y);
    }
    m_in.Enumerate ();
    m_out.Enumerate ();
}

//-----------------------------------------------------------------------------
void  
BSP_Tree_Node::Push (const EdgeList& edgeList, EdgeList& result, HClass keep)
{
    // push the edge list across the splitting edge for this node
    EdgeList    inside;
    EdgeList    outside;
    Push (edgeList, inside, result, outside);
    
    // push the split edge list through the two sub-trees
    if (inside.size () > 0)
        m_in.Push (inside, result, keep, HC_INSIDE);
    if (outside.size () > 0)
        m_out.Push (outside, result, keep, HC_OUTSIDE);
}

//-----------------------------------------------------------------------------
void  
BSP_Tree_Node::Push (const EdgeList& edgeList, EdgeList& inside, EdgeList& coincident, EdgeList& outside)
{
    // we're going to loop over the edges off the input list, either storing 
    // them in the coincident list, splitting them into the in and out lists,
    // or outright moving them to the in list or out list.
    EdgeList::const_iterator    iter = edgeList.begin ();
    EdgeList::const_iterator    iterEnd = edgeList.end ();
    while (iter != iterEnd)
    {
        const Edge& edge = *iter++;
        
        // figure out which side of the splitting edge the two points of the
        // test edge lie on. There are basically 4 possibilities:
        // 1) the points span the splitting edge
        // 2) both points are coincident with the splitting edge
        // 3) both points lie on the same side of the splitting edge
        // 4) one of the points is coincident with the splitting edge, we
        //    treat this the same as #3
        HClass  aTest = LineTest (m_splitEdge, edge.a);
        HClass  bTest = LineTest (m_splitEdge, edge.b);
        
        if (aTest * bTest == HC_SPANNING)
        {
            // case 1: HC_SPANNING is -1, and aTest * bTest will be -1 iff
            // one of them is HC_INSIDE (1) and the other is HC_OUTSIDE (-1),
            // otherwise the product is 1 or 0. We need to compute the 
            // intersection point and build two new edges
            Point   intersectionPoint = IntersectSegments (m_splitEdge, edge);
            if (aTest == HC_INSIDE)
            {
                inside.push_back (Edge (edge.a, intersectionPoint));
                outside.push_back (Edge (intersectionPoint, edge.b));
            }
            else // aTest == HC_OUTSIDE
            {
                outside.push_back (Edge (edge.a, intersectionPoint));
                inside.push_back (Edge (intersectionPoint, edge.b));
            }
        }
        else if (aTest + bTest == HC_COINCIDENT)
        {
            // case 2: aTest + bTest could be 0 if one of them was 1 and the
            // other was -1. but we know that isn't the case because that would
            // have been caught as part of case 1 testing above, so a sum of 0
            // means only that both operands are 0 to start with.
            coincident.push_back (edge);
        }
        else
        {
            // cases 3 and 4: we know that aTest and bTest are either the same
            // value, or that one of them is 0 (HC_COINCIDENT). If this is 
            // case 4, we convert it to case 3 by adding the two values and
            // working only on aTest.
            if (aTest != bTest)
                aTest += bTest;
            if (aTest == HC_INSIDE)
                inside.push_back (edge);
            else // (aTest == HC_OUTSIDE)
                outside.push_back (edge);
        }
    }
}

//-----------------------------------------------------------------------------
void    Test (void)
{
    Point       a (+1, +1);
    Point       b (-1, +1);
    Point       c (-1, -1);
    Point       d (+1, -1);
    
    EdgeList    edgeList;
    edgeList.push_back (Edge (a, b));
    edgeList.push_back (Edge (b, c));
    edgeList.push_back (Edge (c, d));
    edgeList.push_back (Edge (d, a));
    
    BSP_Tree    bsp_tree;
    bsp_tree.Insert (edgeList, HC_OUTSIDE);

    edgeList.clear ();
    Vector      offset (0.5, 0.5);
    a += offset;
    b += offset; 
    c += offset;
    d += offset;
    edgeList.push_back (Edge (a, b));
    edgeList.push_back (Edge (b, c));
    edgeList.push_back (Edge (c, d));
    edgeList.push_back (Edge (d, a));
    
    bsp_tree.Insert (edgeList, HC_OUTSIDE);
    
    bsp_tree.Reduce ();
    bsp_tree.Enumerate ();
}

//-----------------------------------------------------------------------------
