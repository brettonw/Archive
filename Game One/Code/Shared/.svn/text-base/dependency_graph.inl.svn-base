//-----------------------------------------------------------------------------
// Copyright (C) 1997-2002 Bretton Wade
// All rights reserved.
//-----------------------------------------------------------------------------

#ifndef     _DEPENDENCY_GRAPH_INL_
#define     _DEPENDENCY_GRAPH_INL_

//-----------------------------------------------------------------------------
// include files
//-----------------------------------------------------------------------------
#ifndef     _DEPENDENCY_GRAPH_H_
#include    "dependency_graph.h"
#endif  //  _DEPENDENCY_GRAPH_H_

//-----------------------------------------------------------------------------
// template methods
//-----------------------------------------------------------------------------
template <class aType>
void
DependencyGraph<aType>::AddNode (const Text& name, const aType& item)
{
    Assert (m_nodes.find (name) == m_nodes.end ());
    m_nodes[name] = item;
}

//-----------------------------------------------------------------------------
template <class aType>
void
DependencyGraph<aType>::RemoveNode (const Text& name)
{
    Assert (m_nodes.find (name) != m_nodes.end ());
    m_nodes.erase (name);
}

//-----------------------------------------------------------------------------
template <class aType>
bool
DependencyGraph<aType>::HasNode (const Text& name) const
{
    return (m_nodes.find (name) != m_nodes.end ());
}

//-----------------------------------------------------------------------------
template <class aType>
const aType&
DependencyGraph<aType>::GetNode (const Text& name) const
{
    TextMap<aType>::const_iterator  found = m_nodes.find (name);
    Assert (found != m_nodes.end ());
    return found->second;
}

//-----------------------------------------------------------------------------
template <class aType>
aType&
DependencyGraph<aType>::GetNode (const Text& name)
{
    Assert (m_nodes.find (name) != m_nodes.end ());
    return m_nodes[name];
}

//-----------------------------------------------------------------------------
template <class aType>
void
DependencyGraph<aType>::AddEdge (const Text& from, const Text& to)
{
    m_edgesFrom[from].push_back (to);
    m_edgesTo[to].push_back (from);
}

//-----------------------------------------------------------------------------
template <class aType>
void
DependencyGraph<aType>::RemoveEdge (const Text& from, const Text& to)
{
    m_edgesFrom[from].remove (to);
    m_edgesTo[to].remove (from);
}

//-----------------------------------------------------------------------------
template <class aType>
const TextList&
DependencyGraph<aType>::GetEdgesFrom (const Text& from) const
{
    return m_edgesFrom[from];
}

//-----------------------------------------------------------------------------
template <class aType>
const TextList&
DependencyGraph<aType>::GetEdgesTo (const Text& to) const
{
    return m_edgesTo[to];
}

//-----------------------------------------------------------------------------
template <class aType>
void
DependencyGraph<aType>::FindDependencies (TextList& list) const
{
    // clear the node state table to PENDING
    TextMap<NodeState>              state;
    TextMap<aType>::const_iterator  nodeIter = m_nodes.begin ();
    while (nodeIter != m_nodes.end ())
    {
        state[nodeIter->first] = PENDING;
        ++nodeIter;
    }
    
    // clear the list target
    list.clear ();
    
    // now walk the topology
    nodeIter = m_nodes.begin ();
    while (nodeIter != m_nodes.end ())
    {
        Text    name = nodeIter->first;
        if (state[name] == PENDING)
            Visit (name, state, list);
        ++nodeIter;
    }
}

//-----------------------------------------------------------------------------
template <class aType>
void
DependencyGraph<aType>::Visit (const Text& name, TextMap<NodeState>& state, TextList& list) const
{
    state[name] = LIVE;
    TextMap<TextList>::const_iterator   edgeListIter = m_edgesFrom.find (name);
    if (edgeListIter != m_edgesFrom.end ())
    {
        const TextList&                         edgesFrom = edgeListIter->second;
        TextList::const_iterator                edgeIter = edgesFrom.begin ();
        while (edgeIter != edgesFrom.end ())
        {
            Text    to = *edgeIter++;
            Assert (state[to] != LIVE);
            if (state[to] == PENDING)
                Visit (to, state, list);
        }
    }
    state[name] = DEAD;
    list.push_front (name);
}

//-----------------------------------------------------------------------------

#endif  //  _DEPENDENCY_GRAPH_INL_
