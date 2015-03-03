//-----------------------------------------------------------------------------
// Copyright (C) 2006 Bretton Wade, All Rights Reserved
//-----------------------------------------------------------------------------

#ifndef     _LIST_NODE_H_
#define     _LIST_NODE_H_

//-----------------------------------------------------------------------------
// classes
//-----------------------------------------------------------------------------
template<class aType>
class ListNode
{
    protected:
        ListNode*       m_pPrev;
        ListNode*       m_pNext;
        
    public:
        /* void */      ListNode (void) {}
        
        void            InitSentinel (void);
        void            InitNull (void);
        
        void            AddToList (ListNode* pAfter);
        void            RemoveFromList (void);
        
        aType*          GetNext (void);
        const aType*    GetNext (void) const;
        aType*          GetPrev (void);
        const aType*    GetPrev (void) const;
};

//-----------------------------------------------------------------------------
// includes
//-----------------------------------------------------------------------------
#ifndef     _LIST_NODE_INL_
#include    "list_node.inl"
#endif  //  _LIST_NODE_INL_

//-----------------------------------------------------------------------------

#endif  //  _LIST_NODE_H_
