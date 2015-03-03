//-----------------------------------------------------------------------------
// Copyright (C) 2006 Bretton Wade, All Rights Reserved
//-----------------------------------------------------------------------------

#ifndef     _LIST_NODE_INL_
#define     _LIST_NODE_INL_

//-----------------------------------------------------------------------------
// includes
//-----------------------------------------------------------------------------
#ifndef     _LIST_NODE_H_
#include    "list_node.h"
#endif  //  _LIST_NODE_H_

//-----------------------------------------------------------------------------
// inline methods
//-----------------------------------------------------------------------------
template<class aType>
inline
void
ListNode<aType>::InitSentinel (void)
{
    m_pNext = this;
    m_pPrev = this;
}

//-----------------------------------------------------------------------------
template<class aType>
inline
void
ListNode<aType>::InitNull (void)
{
    m_pNext = 0;
    m_pPrev = 0;
}

//-----------------------------------------------------------------------------
template<class aType>
inline
void        
ListNode<aType>::AddToList (ListNode<aType>* pAfter)
{
    // set the node up for it's neighbors in the list
    m_pNext = pAfter->m_pNext;
    m_pPrev = pAfter;
    
    // set up the neighbors for this node to take its place in the list
    m_pNext->m_pPrev = this;
    m_pPrev->m_pNext = this;
}

//-----------------------------------------------------------------------------
template<class aType>
inline
void
ListNode<aType>::RemoveFromList (void)
{
    // remove this node from the list
    m_pNext->m_pPrev = m_pPrev;
    m_pPrev->m_pNext = m_pNext;
    
    // note that the node itself is not touched
}

//-----------------------------------------------------------------------------
template<class aType>
inline
aType*
ListNode<aType>::GetNext (void) 
{
    return s_cast<aType*> (m_pNext);
}

//-----------------------------------------------------------------------------
template<class aType>
inline
const aType*
ListNode<aType>::GetNext (void) const
{
    return s_cast<aType*> (m_pNext);
}

//-----------------------------------------------------------------------------
template<class aType>
inline
aType*
ListNode<aType>::GetPrev (void) 
{
    return s_cast<aType*> (m_pPrev);
}

//-----------------------------------------------------------------------------
template<class aType>
inline
const aType*
ListNode<aType>::GetPrev (void) const
{
    return s_cast<aType*> (m_pPrev);
}

//-----------------------------------------------------------------------------

#endif  //  _LIST_NODE_INL_
