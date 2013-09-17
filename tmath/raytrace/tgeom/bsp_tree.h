//-----------------------------------------------------------------------------
// Copyright (C) 2002-2003 Bretton Wade, All rights reserved
//-----------------------------------------------------------------------------

#ifndef     _BSP_TREE_H_
#define     _BSP_TREE_H_

//-----------------------------------------------------------------------------
// includes
//-----------------------------------------------------------------------------
#ifndef     _INTERSECT_H_
#include    "intersect.h"
#endif  //  _INTERSECT_H_

#ifndef     _META_H_
#include    "meta.h"
#endif  //  _META_H_

#ifndef     _FREE_LIST_H_
#include    "free_list.h"
#endif  //  _FREE_LIST_H_

#ifndef     _STD_ALLOCATOR_H_
#include    "std_allocator.h"
#endif  //  _STD_ALLOCATOR_H_

//-----------------------------------------------------------------------------
// Binary Space Partitioning template class.
//-----------------------------------------------------------------------------
template<typename Real, int iDim>
class BSP
{
    // public interface for BSP
    // class BPoint;
    // class BType;
    // class BRep : public List<BType> {};
    // class Hyperplane;
   
    // class Tree
    // /* void */      Tree (void);
    // /* void */      ~Tree (void);
    // void            Add (BRep& bRep);
    // void            Union (BRep& bRep, bool bReduce = true);
    // void            Intersection (BRep& bRep);
    // void            Difference (BRep& bRep);
    // void            Enumerate (BRep& result);
    // void            EnumerateNearToFar (BRep& result, const BPoint& bPoint);
    // void            EnumerateFarToNear (BRep& result, const BPoint& bPoint);
    
        //---------------------------------------------------------------------
    public:
        //---------------------------------------------------------------------
        class BPoint
        {
            private:
                //-------------------------------------------------------------
                Real        tuple[iDim];
                
                //-------------------------------------------------------------
            public:
                //-------------------------------------------------------------
                /* void */  BPoint (void) {}

                //-------------------------------------------------------------
                /* void */  BPoint (const BPoint& point)
                {
                    Meta<Real>::Copy<iDim> (point.tuple, tuple);
                }

                //-------------------------------------------------------------
                Real&       operator[] (uInt i)
                {
                    Assert (i < iDim);
                    return tuple[i];
                }

                //-------------------------------------------------------------
                const Real& operator[] (uInt i) const
                {
                    Assert (i < iDim);
                    return tuple[i];
                }

                //-------------------------------------------------------------
                BPoint&     operator = (const BPoint& point)
                {
                    Meta<Real>::Copy<iDim> (point.tuple, tuple);
                    return *this;
                }

                //-------------------------------------------------------------
                Real        operator DOT (const BPoint& point) const
                {
                    return MultiplyAndAdd<iDim> (tuple, point.tuple);
                }

                //-------------------------------------------------------------
                BPoint      operator - (void) const
                {
                    BPoint  dst;
                    Meta<Real>::CopyAndNegate<iDim> (tuple, dst.tuple);
                    return dst;
                }

                //-------------------------------------------------------------
                BPoint      operator - (const BPoint& point) const
                {
                    BPoint  dst;
                    Meta<Real>::Sub<iDim> (tuple, point.tuple, dst.tuple);
                    return dst;
                }

                //-------------------------------------------------------------
                BPoint      operator + (const BPoint& point) const
                {
                    BPoint  dst;
                    Meta<Real>::Add<iDim> (tuple, point.tuple, dst.tuple);
                    return dst;
                }

                //-------------------------------------------------------------
                BPoint      operator * (Real scalar) const
                {
                    BPoint  dst;
                    Meta<Real>::Multiply<iDim> (tuple, scalar, dst.tuple);
                    return dst;
                }

                //-------------------------------------------------------------
                BPoint      operator / (Real scalar) const
                {
                    BPoint  dst;
                    Meta<Real>::Multiply<iDim> (tuple, static_cast<Real> (1) / scalar, dst.tuple);
                    return dst;
                }

                //-------------------------------------------------------------
                bool        operator == (const BPoint& bPoint) const
                {
                    return Meta<Real>::Equality (tuple, bPoint.tuple);
                }

                //-------------------------------------------------------------
                bool        operator != (const BPoint& bPoint) const
                {
                    return Meta<Real>::Inequality (tuple, bPoint.tuple);
                }

                //-------------------------------------------------------------
                void        DebugPrint (void) const
                {
                    ::DebugPrint ("(");
                    Meta<Real>::Print<iDim> (tuple);
                    ::DebugPrint (")\n");
                }

                //-------------------------------------------------------------
        };
        
        //---------------------------------------------------------------------
        class BType
        {
            private:
                //-------------------------------------------------------------
                BPoint      point[iDim];
                
                //-------------------------------------------------------------
            public:
                //-------------------------------------------------------------
                /* void */  BType (void) {}

                //-------------------------------------------------------------
                /* void */  BType (const BType& bType)
                {
                    Meta<BPoint>::Copy<iDim> (bType.point, point);
                }

                //-------------------------------------------------------------
                void        Invert (void);

                //-------------------------------------------------------------
        };

        //---------------------------------------------------------------------
        class Hyperplane
        {
            private:
                //-------------------------------------------------------------
                BPoint      normal;
                Real        offset;
                
                //-------------------------------------------------------------
            public:
                //-------------------------------------------------------------
                /* void */  Hyperplane (const BPoint& _normal, Real _offset) :
                    normal (_normal), offset (_offset) {}

                //-------------------------------------------------------------
                /* void */  Hyperplane (const BPoint& _normal, const BPoint& sample) :
                    normal (_normal), offset (-(sample dot normal)) {}

                //-------------------------------------------------------------
                Real        Classify  (const BPoint& point)
                {
                    return (point dot normal) + offset;
                }

                //-------------------------------------------------------------
        };
        
        //---------------------------------------------------------------------
        class BRep : public List<BType> 
        {
            public:
                //-------------------------------------------------------------
                Hyperplane  ComputeHyperplane (void);

                //-------------------------------------------------------------
        };

        //---------------------------------------------------------------------
    private:
        //---------------------------------------------------------------------
        enum
        {
            HC_INSIDE = TURNS_LEFT, 
            HC_COINCIDENT = COLINEAR, 
            HC_OUTSIDE = TURNS_RIGHT,
            HC_SPANNING = SPANNING
        };

        //---------------------------------------------------------------------
    public:
        //---------------------------------------------------------------------
        class Tree
        {
            friend class Node;
            private:
                //-------------------------------------------------------------
                Node*           m_pNode;
                // XXX idea to store a "pending" list in sub trees that aren't
                // XXX actually built into a tree until they need to be 
                // XXX classified against - though this isn't going to gain
                // XXX anything when doing CSG via the tree.

                //-------------------------------------------------------------
            public:
                //-------------------------------------------------------------
                /* void */      Tree (void) : m_pNode (0) 
                {
                }
                
                //-------------------------------------------------------------
                /* void */      ~Tree (void)
                {
                    if (m_pNode) delete m_pNode;
                }

                //-------------------------------------------------------------
                void            Add (BRep& bRep)
                {
                    // insert the BRep into the tree keeping everything and not
                    // performing any reduction
                    Insert (bRep, HC_KEEP_ALL, HC_OUTSIDE, false);
                }

                //-------------------------------------------------------------
                void            Union (BRep& bRep, bool bReduce = true)
                {
                    // insert the BRep into the tree keeping only parts that land
                    // "outside", and parametrically decide whether or not to
                    // eliminate interior edges from the existing tree.
                    Insert (bRep, HC_OUTSIDE, HC_OUTSIDE, bReduce);
                }

                //-------------------------------------------------------------
                void            Intersection (BRep& bRep)
                {
                    // insert the BRep into the tree keeping only parts that land
                    // "inside", and then eliminate exterior edges from the 
                    // existing tree
                    Insert (bRep, HC_INSIDE, HC_INSIDE, true);
                }

                //-------------------------------------------------------------
                void            Difference (BRep& bRep)
                {
                    // The difference is the intersection of the source tree with 
                    // the inverse of the BRep.
                    
                    // the boundary must first be inverted, so the boundary type 
                    // must implement a method Invert with void signatures.
                    BRep            copy;
                    copy.insert (copy.end (), bRep.begin (), bRep.end ());
                    BRep::iterator  iter = copy.begin ();
                    BRep::iterator  iterEnd = copy.end ();
                    while (iter != iterEnd)
                    {
                        BType&  bType = *iter++;
                        bType.Invert ();
                    }
                    
                    // insert the BRep into the tree keeping only parts that land
                    // "inside", and then eliminate exterior edges from the 
                    // existing tree
                    Insert (copy, HC_INSIDE, HC_INSIDE, true);
                }

                //-------------------------------------------------------------
                void            Enumerate (BRep& result)
                {
                    if (m_pNode) m_pNode->Enumerate (result);
                }

                //-------------------------------------------------------------
                void            EnumerateNearToFar (BRep& result, const BPoint& bPoint)
                {
                    if (m_pNode) m_pNode->EnumerateNearToFar (result);
                }

                //-------------------------------------------------------------
                void            EnumerateFarToNear (BRep& result, const BPoint& bPoint)
                {
                    if (m_pNode) m_pNode->EnumerateFarToNear (result);
                }

                //-------------------------------------------------------------
            private:
                //-------------------------------------------------------------
                void            Insert (BRep& bRep, sInt keep, sInt current, bool bReduce)
                {
                    // check to see there is a sub-tree rooted here. 
                    if (m_pNode)
                    {
                        // everything is good, insert the brep into the sub-tree
                        m_pNode->Insert (bRep, keep, bReduce);
                    }
                    else
                    {
                        // before we try to create a new sub-tree, we want to know if we
                        // really want to keep these bits.
                        if ((keep == HC_KEEP_ALL) or (current == keep))
                        {
                            // create a new sub tree, keeping everything left in the brep
                            m_pNode = new BSP<Real, iDim>::Node (bRep.ComputeHyperplane ());
                            m_pNode->Insert (bRep, HC_KEEP_ALL, bReduce);
                        }
                    }
                }

                //-------------------------------------------------------------
                void            Push (const BRep& bRep, BRep& result, sInt keep, sInt current)
                {
                    // make sure there is a boundary to deal with
                    Assert (bRep.size () > 0);
                    
                    // check to see if there is a valid node, and if so push
                    // the brep through it, if not, then this is a leaf and we
                    // want to decide if we should keep it.
                    if (m_pNode)
                        m_pNode->Push (bRep, result, keep);
                    else if ((keep == HC_KEEP_ALL) or (current == keep))
                        result.insert (result.end (), bRep.begin (), bRep.end ());
                }
                
                //-------------------------------------------------------------
        };

        //---------------------------------------------------------------------
    private:
        //---------------------------------------------------------------------
        struct Node : public FreeList<Node>
        {
            //-----------------------------------------------------------------
            Tree            m_in;
            Tree            m_out;
            Hyperplane      m_hyperplane;
            BRep            m_coincidentList;
            
            //-----------------------------------------------------------------
            /* void */      Node (Hyperplane& hyperplane) : m_hyperplane (hyperplane)
            {
            }

            //-----------------------------------------------------------------
            /* void */      ~Node (void)
            {
            }

            //-----------------------------------------------------------------
            void            Insert (const BRep& bRep, sInt keep, bool bReduce)
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
            void            Enumerate (BRep& result)
            {
                result.insert (result.end (), m_coincidentList.begin (), m_coincidentList.end ());
                m_in.Enumerate (result);
                m_out.Enumerate (result);
            }
            
            //-----------------------------------------------------------------
              void            EnumerateNearToFar (BRep& result, const BPoint& bPoint)
            {
                // classify the point to see which way to go
                Real    classification = m_hyperplane.Classify (bPoint);
                Real    epsilon = GetEpsilon<Real> ();
                if (classification < -epsilon)
                {
                    // the point is on the inside
                    m_in.Enumerate (result);
                    result.insert (result.end (), m_coincidentList.begin (), m_coincidentList.end ());
                    m_out.Enumerate (result);
                }
                else (classification > epsilon)
                {
                    // the point is on the outside
                    m_out.Enumerate (result);
                    result.insert (result.end (), m_coincidentList.begin (), m_coincidentList.end ());
                    m_in.Enumerate (result);
                }
                else
                {
                    // the point is on the hyperplane
                    //result.insert (result.end (), m_coincidentList.begin (), m_coincidentList.end ());
                    m_in.Enumerate (result);
                    m_out.Enumerate (result);
                }
            }
            
            //-----------------------------------------------------------------
            void            EnumerateFarToNear (BRep& result, const BPoint& bPoint)
            {
                // classify the point to see which way to go
                Real    classification = m_hyperplane.Classify (bPoint);
                Real    epsilon = GetEpsilon<Real> ();
                if (classification < -epsilon)
                {
                    // the point is on the inside
                    m_out.Enumerate (result);
                    result.insert (result.end (), m_coincidentList.begin (), m_coincidentList.end ());
                    m_in.Enumerate (result);
                }
                else (classification > epsilon)
                {
                    // the point is on the outside
                    m_in.Enumerate (result);
                    result.insert (result.end (), m_coincidentList.begin (), m_coincidentList.end ());
                    m_out.Enumerate (result);
                }
                else
                {
                    // the point is on the hyperplane
                    m_in.Enumerate (result);
                    m_out.Enumerate (result);
                    //result.insert (result.end (), m_coincidentList.begin (), m_coincidentList.end ());
                }
            }
            
            //-----------------------------------------------------------------
            void            Push (const BRep& bRep, BRep& result, sInt keep)
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
        };
        
        //---------------------------------------------------------------------
};

//-----------------------------------------------------------------------------

#endif  //  _BSP_TREE_H_