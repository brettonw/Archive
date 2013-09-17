//-----------------------------------------------------------------------------
// Copyright (C) 2006 Bretton Wade, All Rights Reserved
//-----------------------------------------------------------------------------

#ifndef     _LIST_NODE_H_
#define     _LIST_NODE_H_

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// BEGIN DECLARATIONS
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

//-----------------------------------------------------------------------------
// classes
//-----------------------------------------------------------------------------
struct ListNode
{
    ListNode*       pPrev;
    ListNode*       pNext;
    
    /* void */      ListNode (void) {}
    void            InitSentinel (void);
    void            InitNull (void);
    void            AddToList (ListNode* pAfter);
    void            RemoveFromList (void);
};

//-----------------------------------------------------------------------------
// inline methods
//-----------------------------------------------------------------------------
inline
void
ListNode::InitSentinel (void)
{
    pNext = this;
    pPrev = this;
}

//-----------------------------------------------------------------------------
inline
void
ListNode::InitNull (void)
{
    pNext = 0;
    pPrev = 0;
}

//-----------------------------------------------------------------------------
inline
void        
ListNode::AddToList (ListNode* pAfter)
{
    // set the node up for it's neighbors in the list
    pNext = pAfter->pNext;
    pPrev = pAfter;
    
    // set up the neighbors for this node to take its place in the list
    pNext->pPrev = this;
    pPrev->pNext = this;
}

//-----------------------------------------------------------------------------
inline
void        
ListNode::RemoveFromList (void)
{
    // remove this node from the list
    pNext->pPrev = pPrev;
    pPrev->pNext = pNext;
    
    // note that the node itself is not touched
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// END DECLARATIONS
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#endif  //  _LIST_NODE_H_
