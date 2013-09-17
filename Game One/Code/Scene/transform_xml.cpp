//-----------------------------------------------------------------------------
// Copyright (C) 1997-2006 Bretton Wade
// All Rights Reserved
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// include files
//-----------------------------------------------------------------------------
#include    "precompile.h"
#include    "transform_xml.h"

//-----------------------------------------------------------------------------
void
ImportTransforms (D3DXMATRIX* pMatrixOut, const PtrToXMLNode& pXMLNode)
{
    // clear to identity for starters
    D3DXMatrixIdentity (pMatrixOut);

    // check to see if the node is valid
    if (pXMLNode)
    {
        // fetch the transform list, and check it for validity
        const XMLNodeList*  pNodeList = pXMLNode->GetChildren ("Transform");
        if (pNodeList)
        {
            // read the transforms in order and concatenate them
            XMLNodeList::const_iterator iter = pNodeList->begin ();
            XMLNodeList::const_iterator end = pNodeList->end ();
            while (iter != end)
            {
                // fetch the current transform, and type its
                // type attribute
                PtrToXMLNode    pTransformNode = *iter++;
                Text            transformType = pTransformNode->GetAttribute ("type");
                
                // a place to put the new transform
                D3DXMATRIX      transformMatrix;
                
                // check the transform type for the appropriate interpretation
                if (transformType == "identity")
                {
                    D3DXMatrixIdentity (&transformMatrix);
                }
                else if (transformType == "scale")
                {
                    // read the x, y, and z scale values, and set the matrix
                    float   x, y, z;
                    sscanf (pTransformNode->GetValue (), "(%f, %f, %f)", &x, &y, &z);
                    D3DXMatrixScaling (&transformMatrix, x, y, z);
                }
                else if (transformType == "rotation")
                {
                    // read the x, y, and z axis values, and the angle value
                    float   x, y, z, angle;
                    sscanf (pTransformNode->GetValue (), "(%f, %f, %f) %f", &x, &y, &z, &angle);
                    
                    // convert the angle from degrees to radians and set the matrix
                    angle = (angle / 180) * D3DX_PI;
                    D3DXMatrixRotationAxis (&transformMatrix, &D3DXVECTOR3 (x, y, z), angle);
                }
                else if (transformType == "translation")
                {
                    // read the x, y, and z translation values, and set the matrix
                    float   x, y, z;
                    sscanf (pTransformNode->GetValue (), "(%f, %f, %f)", &x, &y, &z);
                    D3DXMatrixTranslation (&transformMatrix, x, y, z);
                }
                else if (transformType == "matrix")
                {
                    // read the whole matrix
                    float   m[16];
                    sscanf (pTransformNode->GetValue (), "(%f, %f, %f, %f) (%f, %f, %f, %f) (%f, %f, %f, %f) (%f, %f, %f, %f)", 
                        m +  0, m +  1, m +  2, m +  3, 
                        m +  4, m +  5, m +  6, m +  7, 
                        m +  8, m +  9, m + 10, m + 11, 
                        m + 12, m + 13, m + 14, m + 15);
                    transformMatrix = D3DXMATRIX (m);
                }
                else
                {
                    Assert (false and "Unsupported transform type");
                    D3DXMatrixIdentity (&transformMatrix);
                }
                
                // concatenate the current transform with the final one
                D3DXMatrixMultiply (pMatrixOut, pMatrixOut, &transformMatrix);
            }
        }
    }
}

//-----------------------------------------------------------------------------
