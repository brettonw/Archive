//------------------------------------------------------------------------------
//	Copyright (C) 1997-2000 Bretton Wade
//	All Rights Reserved
//------------------------------------------------------------------------------

#ifndef		_BW_GRAPH_OF_H_
#define		_BW_GRAPH_OF_H_

//--------------------------------------------------------------------------
//	includes
//--------------------------------------------------------------------------
#ifndef		_BW_GRAPH_NODE_OF_H_
#include	"graph_node_of.h"
#endif	//	_BW_GRAPH_NODE_OF_H_

#ifndef		_BW_HASH_TABLE_OF_H_
#include	"hash_table_of.h"
#endif	//	_BW_HASH_TABLE_OF_H_

#ifndef		_BW_ITERATOR_ON_HASH_TABLE_OF_H_
#include	"iterator_on_hash_table_of.h"
#endif	//	_BW_ITERATOR_ON_HASH_TABLE_OF_H_

#ifndef		_BW_TEXT_H_
#include	"text.h"
#endif	//	_BW_TEXT_H_

#ifndef		_BW_EXCEPTION_H_
#include	"exception.h"
#endif	//	_BW_EXCEPTION_H_

//--------------------------------------------------------------------------
//	exception definitions
//--------------------------------------------------------------------------
DEFINE_EXCEPTION (NodeNotFound);

//--------------------------------------------------------------------------
//	forward class declarations
//--------------------------------------------------------------------------
template	<class aType>
class	IteratorOnGraphOf;

//--------------------------------------------------------------------------
//	class definitions
//--------------------------------------------------------------------------
template	<class aType>
class GraphOf
{
friend	class	IteratorOnGraphOf<aType>;
	public:
												~GraphOf (void);
		void									AddNode (const Text& name, const aType& item);
		void									AddEdge (const Text& source, const Text& destination);
	protected:
		HashTableOf<Text, GraphNodeOf<aType>*>	myNodes;
};

//--------------------------------------------------------------------------
//	template methods
//--------------------------------------------------------------------------
template	<class aType>
GraphOf<aType>::~GraphOf (void)
{
	IteratorOnHashTableOf<Text, GraphNodeOf<aType>*>	iterator (myNodes);
	while (not iterator.IsDone ())
	{
		delete iterator.GetCurrentItem ();
		iterator.GotoNextItem ();
	}
}

//--------------------------------------------------------------------------
template	<class aType>
void	GraphOf<aType>::AddNode (const Text& name, const aType& item)
{
	assert (not myNodes.Contains (name));
	myNodes.PutItem (name, new GraphNodeOf<aType> (item));
}

//--------------------------------------------------------------------------
template	<class aType>
void	GraphOf<aType>::AddEdge (const Text& source, const Text& destination)
{
    GraphNodeOf<aType>* sourceNode;
    try 
    {
        sourceNode = myNodes.GetItem (source);
    }
	catch (KeyNotFound)
	{
	    throw NodeNotFound (cString (source));
	}
	
	GraphNodeOf<aType>* destinationNode;
    try 
    {
	    destinationNode = myNodes.GetItem (destination);
	}
	catch (KeyNotFound)
	{
	    throw NodeNotFound (cString (destination));
	}
	
	sourceNode->AddEdgeTo (destinationNode);
}

//--------------------------------------------------------------------------

#endif	//	_BW_GRAPH_OF_H_
