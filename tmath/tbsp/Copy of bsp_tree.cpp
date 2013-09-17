//-----------------------------------------------------------------------------
// Copyright (C) 2003 Microsoft, All rights reserved
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// includes
//-----------------------------------------------------------------------------
#include    "tgeom.h"
#include    "intersect.h"

//-----------------------------------------------------------------------------
// Binary Space Partitioning template class. The user provides a boundary type
// and the class provides a boundary representation type called BRep that is a
// list of the boundary types. The user must also specialize the template member 
// functions BSP_2D::Node::Enumerate and BSP_2D::Node::Push to iterate 
// over the boundary representation lists provided and make appropriate 
// decisions.
//-----------------------------------------------------------------------------
template<typename Real, typename BType>
class BSP
{
    // public interface for BSP::Tree
    // /* void */      Tree (void);
    // /* void */      ~Tree (void);
    // void            Add (BRep& bRep);
    // void            Union (BRep& bRep, bool bReduce = true);
    // void            Intersection (BRep& bRep);
    // void            Difference (BRep& bRep);
    // void            Enumerate (BRep& bRep);
    
    public:
    class BRep : public std::list<BType> {};

    private:
    typedef sInt                HClass;
    enum
    {
        HC_INSIDE = TURNS_LEFT, 
        HC_COINCIDENT = COLINEAR, 
        HC_OUTSIDE = TURNS_RIGHT,
        HC_SPANNING = SPANNING,
        HC_KEEP_ALL = 2
    };
    class Node;
    
    public:    
    class Tree
    {
        friend class Node;
        public:
            //-----------------------------------------------------------------
            /* void */      Tree (void) : m_pNode (0) 
            {
            }
            
            //-----------------------------------------------------------------
            /* void */      ~Tree (void)
            {
                if (m_pNode) delete m_pNode;
            }

            //-----------------------------------------------------------------
            void            Add (BRep& bRep)
            {
                // insert the BRep into the tree keeping everything and not
                // performing any reduction
                Insert (bRep, HC_KEEP_ALL, HC_OUTSIDE, false);
            }

            //-----------------------------------------------------------------
            void            Union (BRep& bRep, bool bReduce = true)
            {
                // insert the BRep into the tree keeping only parts that land
                // "outside", and parametrically decide whether or not to
                // eliminate interior edges from the existing tree.
                Insert (bRep, HC_OUTSIDE, HC_OUTSIDE, bReduce);
            }

            //-----------------------------------------------------------------
            void            Intersection (BRep& bRep)
            {
                // insert the BRep into the tree keeping only parts that land
                // "inside", and then eliminate exterior edges from the 
                // existing tree
                Insert (bRep, HC_INSIDE, HC_INSIDE, true);
            }

            //-----------------------------------------------------------------
            void            Difference (BRep& bRep)
            {
                // The difference is the intersection of the source tree with 
                // the inverse of the BRep.
                
                // the boundary must first be inverted, so the boundary type 
                // must implement a method Invert with void signatures.
                BRep::iterator  iter = bRep.begin ();
                BRep::iterator  iterEnd = bRep.end ();
                while (iter != iterEnd)
                {
                    BType&  bType = *iter++;
                    bType.Invert ();
                }
                
                // insert the BRep into the tree keeping only parts that land
                // "inside", and then eliminate exterior edges from the 
                // existing tree
                Insert (bRep, HC_INSIDE, HC_INSIDE, true);
            }

            //-----------------------------------------------------------------
            void            Enumerate (BRep& bRep)
            {
                if (m_pNode) m_pNode->Enumerate (bRep);
            }

            //-----------------------------------------------------------------
        private:
            //-----------------------------------------------------------------
            void            Insert (BRep& bRep, HClass keep, HClass current, bool bReduce)
            {
                // check to see there is a sub-tree rooted here. If not, we'll need to
                // create one.
                if (not m_pNode)
                {
                    // before we try to create a new sub-tree, we want to know if we
                    // really want to keep these bits. If not, clear out the edgelist
                    if ((keep == HC_KEEP_ALL) or (current == keep))
                    {
                        m_pNode = new BSP<Real, BType>::Node (bRep);
                        m_pNode->Insert (bRep, HC_KEEP_ALL, bReduce);
                    }
                }
                else
                    m_pNode->Insert (bRep, keep, bReduce);
            }

            //-----------------------------------------------------------------
            void            Push (const BRep& bRep, BRep& result, HClass keep, HClass current)
            {
                if (bRep.size () > 0)
                {
                    if (m_pNode)
                        m_pNode->Push (bRep, result, keep);
                    else if (current == keep)
                    {
                        BRep::const_iterator    iter = bRep.begin ();
                        BRep::const_iterator    iterEnd = bRep.end ();
                        while (iter != iterEnd)
                            result.push_back (*iter++);
                    }
                }
            }
            
            //-----------------------------------------------------------------
        private:
            Node*           m_pNode;
    };

    private:
    class Node
    {
        friend class Tree;
        private:
            //-----------------------------------------------------------------
            /* void */      Node (BRep& bRep)
            {
                // choose an edge to divide space with, store it in the node, and remove 
                // it from the edge list. For now, we use the end of the list for speed,
                // but better results will probably be had by choosing a random edge
                m_split = bRep.back ();
                m_coincidentList.push_back (m_split);
                bRep.pop_back ();
            }

            //-----------------------------------------------------------------
            /* void */      ~Node (void)
            {
            }

            //-----------------------------------------------------------------
            void            Insert (const BRep& bRep, HClass keep, bool bReduce)
            {
                // push the edge list across the splitting edge for this node
                BRep    inside;
                BRep    outside;
                Push (bRep, inside, m_coincidentList, outside);
                
                // insert the split edge list in the two sub-trees
                if (inside.size () > 0)
                    m_in.Insert (inside, keep, HC_INSIDE, bReduce);
                if (outside.size () > 0)
                    m_out.Insert (outside, keep, HC_OUTSIDE, bReduce);
                    
                // check to see if we want to reduce this node after completing 
                // the insertion - as we would when doing CSG
                if (bReduce)
                {
                    // push the coincident edge list down through the in node, then push the 
                    // results through the out sub-tree keeping the bits that land outside
                    BRep    inKeep;
                    BRep    outKeep;
                    m_in.Push (m_coincidentList, inKeep, HC_INSIDE, HC_INSIDE);
                    m_out.Push (inKeep, outKeep, HC_OUTSIDE, HC_OUTSIDE);
                    
                    // store the result back to the coincident list
                    m_coincidentList.assign (outKeep.begin (), outKeep.end ());
                }
            }

            //-----------------------------------------------------------------
            void            Enumerate (BRep& bRep);
            
            //-----------------------------------------------------------------
            void            Push (const BRep& bRep, BRep& result, HClass keep)
            {
                // push the edge list across the splitting edge for this node
                BRep    inside;
                BRep    outside;
                Push (bRep, inside, result, outside);
                
                // push the split edge list through the two sub-trees
                if (inside.size () > 0)
                    m_in.Push (inside, result, keep, HC_INSIDE);
                if (outside.size () > 0)
                    m_out.Push (outside, result, keep, HC_OUTSIDE);
            }

            //-----------------------------------------------------------------
            void            Push (const BRep& bRep, BRep& inside, BRep& coincident, BRep& outside);

            //-----------------------------------------------------------------
        private:
            Tree            m_in;
            Tree            m_out;
            BType           m_split;
            BRep            m_coincidentList;
    };
};

//-----------------------------------------------------------------------------
// 1D BSP Tree implementation
//-----------------------------------------------------------------------------
template<typename Real>
struct SpanPt
{
    Real        h;
    bool        bLeft;
    
    /* void */  SpanPt (void) {}
    /* void */  SpanPt (Real _h, bool _bLeft) : h (_h), bLeft (_bLeft) {}
    void        Invert (void) { bLeft = ! bLeft; }
};
typedef BSP<double, SpanPt<double> >    BSP_1D;

//-----------------------------------------------------------------------------
void    BSP_1D::Node::Enumerate (BRep& bRep)
{
    bRep.insert (bRep.end (), m_coincidentList.begin (), m_coincidentList.end ());
    m_in.Enumerate (bRep);
    m_out.Enumerate (bRep);
}

//-----------------------------------------------------------------------------
void    BSP_1D::Node::Push (const BRep& bRep, BRep& inside, BRep& coincident, BRep& outside)
{
    // we're going to loop over the edges of the input list, either storing 
    // them in the coincident list, splitting them into the in and out lists,
    // or outright moving them to the in list or out list.
    BRep::const_iterator    iter = bRep.begin ();
    BRep::const_iterator    iterEnd = bRep.end ();
    while (iter != iterEnd)
    {
        const SpanPt<Real>& point = *iter++;
        
        // Classify the point against the splitter, there are five possible 
        // results from this operation:
        // 1) point and split are coincident
        // point is left of split
        //     2) split faces left: outside
        //     3) split faces right: inside
        // point is right of split
        //     4) split faces left: inside
        //     5) split faces right: outside
        if (FuzzyEquals (point.h, m_split.h))
        {
            // case 1:
            // no redundant adds will be accepted, this is a 1 dimensional
            // space, so there is no "span" to worry about reducing
            //coincident.push_back (point);
        }
        else
        {
            bool    bLeft = point.h < m_split.h;
            if (bLeft != m_split.bLeft)
            {
                // case 3 and 5:
                inside.push_back (point);
            }
            else
            {
                // case 2 and 4:
                outside.push_back (point);
            }
        }
    }
}

//-----------------------------------------------------------------------------
// 2D BSP Tree implementation
//-----------------------------------------------------------------------------
typedef TVector2<double>    Vector;
typedef TPoint2<double>     Point;
typedef TEdge2<double>      Edge;

typedef BSP<double, Edge>   BSP_2D;

//-----------------------------------------------------------------------------
#define UNION_EDGES(x)                                                          \
{                                                                               \
    BRep::const_iterator    iter = m_coincidentList.begin ();                   \
    BRep::const_iterator    iterEnd = m_coincidentList.end ();                  \
    while (iter != iterEnd)                                                     \
    {                                                                           \
        const Edge&         edge = *iter++;                                     \
        Real                dDelta = edge.b.x - edge.a.x;                       \
        BSP_1D::BRep        bRep_1D;                                            \
        bRep_1D.push_back (SpanPt<Real> (edge.a.x, (dDelta > 0)));              \
        bRep_1D.push_back (SpanPt<Real> (edge.b.x, (dDelta < 0)));              \
        bsp_tree.Union (bRep_1D, true);                                         \
    }                                                                           \
}

//-----------------------------------------------------------------------------
void    BSP_2D::Node::Enumerate (BRep& bRep)
{
    // set up a one dimensional bsp tree so we can compute the union of all the
    // coincident edges in the node
    BSP_1D::Tree    bsp_tree;

    // use the splitter to determine a projection of the edge onto the X or Y 
    // axis so we can keep as much precision in computation as possible
    Vector          vDelta = m_split.b - m_split.a;
    if (AbsoluteValue (vDelta.x) > AbsoluteValue (vDelta.y))
    {
        // insert the edges into a 1D BSP tree
        UNION_EDGES (x)
        
        // compute the slope of the splitter
        Real                    dSplitSlope = vDelta.y / vDelta.x;
        
        // enumerate the reduced edge point set and convert them back
        // into edges to emit
        BSP_1D::BRep            bRepEdge;
        bsp_tree.Enumerate (bRepEdge);
        Assert ((bRepEdge.size () bit_and 0x01) == 0);
        BSP_1D::BRep::iterator  iter = bRepEdge.begin ();
        BSP_1D::BRep::iterator  iterEnd = bRepEdge.end ();
        while (iter != iterEnd)
        {
            // there should always be an even number of points in the list,
            // so we can take them off two at a time.
            SpanPt<Real>&   point1 = *iter++;
            SpanPt<Real>&   point2 = *iter++;
            Point           a (point1.h, m_split.a.y + ((point1.h - m_split.a.x) * dSplitSlope));
            Point           b (point2.h, m_split.a.y + ((point2.h - m_split.a.x) * dSplitSlope));
            bRep.push_back (Edge (b, a));
        }
    }
    else // (AbsoluteValue (vDelta.x) < AbsoluteValue (vDelta.y))
    {
        // insert the edges into a 1D BSP tree
        UNION_EDGES (y)
        
        // compute the slope of the splitter
        Real                    dSplitSlope = vDelta.x / vDelta.y;
        
        // enumerate the reduced edge point set and convert them back
        // into edges to emit
        BSP_1D::BRep            bRepEdge;
        bsp_tree.Enumerate (bRepEdge);
        Assert ((bRepEdge.size () bit_and 0x01) == 0);
        BSP_1D::BRep::iterator  iter = bRepEdge.begin ();
        BSP_1D::BRep::iterator  iterEnd = bRepEdge.end ();
        while (iter != iterEnd)
        {
            // there should always be an even number of points in the list,
            // so we can take them off two at a time.
            SpanPt<Real>&   point1 = *iter++;
            SpanPt<Real>&   point2 = *iter++;
            Point           a (m_split.a.x + ((point1.h - m_split.a.y) * dSplitSlope), point1.h);
            Point           b (m_split.a.x + ((point2.h - m_split.a.y) * dSplitSlope), point2.h);
            bRep.push_back (Edge (b, a));
        }
    }
    
    // enumerate the child nodes
    m_in.Enumerate (bRep);
    m_out.Enumerate (bRep);
}

//-----------------------------------------------------------------------------
void    BSP_2D::Node::Push (const BRep& bRep, BRep& inside, BRep& coincident, BRep& outside)
{
    // we're going to loop over the edges of the input list, either storing 
    // them in the coincident list, splitting them into the in and out lists,
    // or outright moving them to the in list or out list.
    BRep::const_iterator    iter = bRep.begin ();
    BRep::const_iterator    iterEnd = bRep.end ();
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
        HClass      aTest = LineTest (m_split, edge.a);
        HClass      bTest = LineTest (m_split, edge.b);
        
        if (aTest * bTest == HC_SPANNING)
        {
            // case 1: HC_SPANNING is -1, and aTest * bTest will be -1 iff
            // one of them is HC_INSIDE (1) and the other is HC_OUTSIDE (-1),
            // otherwise the product is 1 or 0. We need to compute the 
            // intersection point and build two new edges
            Point   intersectionPoint = IntersectSegments (m_split, edge);
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
/*
    SpanPt        a (0, true);
    SpanPt        b (2, false);
    SpanPt        c (1, true);
    SpanPt        d (3, false);
    
    BSP_1D::BRep    bRep;
    bRep.push_back (a);
    bRep.push_back (b);

    BSP_1D::Tree     bsp_tree;
    bsp_tree.Add (bRep);
    
    bRep.clear ();
    DebugPrint ("Before\n");
    bsp_tree.Enumerate (bRep);
    
    bRep.clear ();
    bRep.push_back (c);
    bRep.push_back (d);
    
    bsp_tree.Union (bRep, true);

    bRep.clear ();
    DebugPrint ("After\n");
    bsp_tree.Enumerate (bRep);
*/
    Point       a (2, 2);
    Point       b (0, 2);
    Point       c (0, 0);
    Point       d (2, 0);
    
    BSP_2D::BRep    bRep;
    bRep.push_back (Edge (a, b));
    bRep.push_back (Edge (b, c));
    bRep.push_back (Edge (c, d));
    bRep.push_back (Edge (d, a));
    
    BSP_2D::Tree     bsp_tree;
    bsp_tree.Add (bRep);

    Point       e (3, 1);
    Point       f (0, 1);
    Point       g (0, 0);
    Point       h (3, 0);
    bRep.clear ();
    bRep.push_back (Edge (e, f));
    bRep.push_back (Edge (f, g));
    bRep.push_back (Edge (g, h));
    bRep.push_back (Edge (h, e));
    
    bsp_tree.Intersection (bRep);
    
    Point       i (3, 3);
    Point       j (0, 0);
    Point       k (3, 0);
    bRep.clear ();
    bRep.push_back (Edge (i, j));
    bRep.push_back (Edge (j, k));
    bRep.push_back (Edge (k, i));

    bsp_tree.Union (bRep);
    
    bRep.clear ();
    bsp_tree.Enumerate (bRep);
    
    BSP_2D::BRep::iterator    iter = bRep.begin ();
    BSP_2D::BRep::iterator    iterEnd = bRep.end ();
    while (iter != iterEnd)
    {
        const Edge& edge = *iter++;
        DebugPrint ("Edge: (%f, %f) to (%f, %f)\n", edge.a.x, edge.a.y, edge.b.x, edge.b.y);
    }
}

//-----------------------------------------------------------------------------
