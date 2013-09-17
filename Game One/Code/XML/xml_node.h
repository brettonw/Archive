//-----------------------------------------------------------------------------
// Copyright (C) 1997-2006 Bretton Wade
// All rights reserved.
//-----------------------------------------------------------------------------

#ifndef     _XML_NODE_H_
#define     _XML_NODE_H_

//-----------------------------------------------------------------------------
// include files
//-----------------------------------------------------------------------------
#ifndef     _TEXT_MAP_H_
#include    "text_map.h"
#endif  //  _TEXT_MAP_H_

#ifndef     _FREE_LIST_H_
#include    "free_list.h"
#endif  //  _FREE_LIST_H_

//-----------------------------------------------------------------------------
// useful type definitions
//-----------------------------------------------------------------------------
MAKE_PTR_TO (XMLNode);
typedef List<PtrToXMLNode>              XMLNodeList;
typedef XMLNodeList::const_iterator     XMLNodeListIterator;
typedef TextMap<XMLNodeList>            XMLNodeListMap;
typedef XMLNodeListMap::const_iterator  XMLNodeListMapIterator;

//-----------------------------------------------------------------------------
// class definitions
//-----------------------------------------------------------------------------
class XMLNode : public CountedObject, public FreeList<XMLNode>
{
    public:
        /* void */                  XMLNode (const Text& name);
        /* void */                  XMLNode (const Text& name, const Text& value);
virtual /* void */                  ~XMLNode (void);

        void                        SetName (const Text& name);
        Text                        GetName (void) const;

        void                        SetAttribute (const Text& name, const Text& value);
        Text                        GetAttribute (const Text& name) const;

        void                        SetValue (const Text& name);
        Text                        GetValue (void) const;
        
        void                        PutChild (const PtrToXMLNode& node);
        PtrToXMLNode                GetChild (const Text& name) const;
        uInt                        GetChildCount (const Text& name) const;
        const XMLNodeList*          GetChildren (const Text& name) const;

        
static  Text                        GenerateXML (const PtrToXMLNode& node, const Text& indent = "");
static  PtrToXMLNode                ParseXML (const Text& xml);

static  PtrToXMLNode                FromFile (const Text& fileName);
static  void                        ToFile (const Text& fileName, const PtrToXMLNode& xmlNode);
        
    protected:
        Text                        m_name;
        Text                        m_value;
        TextMap<Text>               m_attributes;
        TextMap<XMLNodeList>        m_children;
};

//-----------------------------------------------------------------------------
// inlines
//-----------------------------------------------------------------------------
#ifndef     _XML_NODE_INL_
#include    "xml_node.inl"
#endif  //  _XML_NODE_INL_

//-----------------------------------------------------------------------------

#endif  //  _XML_NODE_H_
