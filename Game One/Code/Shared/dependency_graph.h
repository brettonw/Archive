//-----------------------------------------------------------------------------
// Copyright (C) 1997-2002 Bretton Wade
// All rights reserved.
//-----------------------------------------------------------------------------

#ifndef     _DEPENDENCY_GRAPH_H_
#define     _DEPENDENCY_GRAPH_H_

//-----------------------------------------------------------------------------
// include files
//-----------------------------------------------------------------------------
#ifndef     _TEXT_MAP_H_
#include    "text_map.h"
#endif  //  _TEXT_MAP_H_

//-----------------------------------------------------------------------------
// useful types
//-----------------------------------------------------------------------------
typedef List<Text>              TextList;

//-----------------------------------------------------------------------------
// class definitions
//-----------------------------------------------------------------------------
template <class aType>
class DependencyGraph
{
    public:
        void                    AddNode (const Text& name, const aType& item);
        void                    RemoveNode (const Text& name);
        
        bool                    HasNode (const Text& name) const;
        const aType&            GetNode (const Text& name) const;
        aType&                  GetNode (const Text& name);
        
        void                    AddEdge (const Text& from, const Text& to);
        void                    RemoveEdge (const Text& from, const Text& to);
        
        const TextList&         GetEdgesFrom (const Text& from) const;
        const TextList&         GetEdgesTo (const Text& to) const;
        
        void                    FindDependencies (TextList& list) const;
        
    protected:
        enum NodeState {PENDING, LIVE, DEAD};
        void                    Visit (const Text& name, TextMap<NodeState>& state, TextList& list) const;
        
    protected:
        TextMap<aType>          m_nodes;
        TextMap<TextList>       m_edgesFrom;
        TextMap<TextList>       m_edgesTo;
};

//-----------------------------------------------------------------------------
// inlines
//-----------------------------------------------------------------------------
#ifndef     _DEPENDENCY_GRAPH_INL_
#include    "dependency_graph.inl"
#endif  //  _DEPENDENCY_GRAPH_INL_

//-----------------------------------------------------------------------------

#endif  //  _DEPENDENCY_GRAPH_H_
