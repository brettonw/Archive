//-----------------------------------------------------------------------------
// Copyright (C) 1997-2006 Bretton Wade
// All Rights Reserved
//-----------------------------------------------------------------------------

#ifndef     _TRANSFORM_XML_H_
#define     _TRANSFORM_XML_H_

//-----------------------------------------------------------------------------
// include files
//-----------------------------------------------------------------------------
#ifndef     _XML_NODE_H_
#include    "xml_node.h"
#endif  //  _XML_NODE_H_

//-----------------------------------------------------------------------------
// functions for reading and writing XML for D3DX types
//-----------------------------------------------------------------------------
void            ImportTransforms (D3DXMATRIX* pMatrixOut, const PtrToXMLNode& pXMLNode);

//-----------------------------------------------------------------------------

#endif  //  _TRANSFORM_XML_H_
