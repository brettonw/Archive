//-----------------------------------------------------------------------------
// Copyright (C) 2002-2004 Bretton Wade, All rights reserved
// This file may be freely transmitted for educational purposes only
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// include files
//-----------------------------------------------------------------------------
#include    "precompile.h"
#include    "cylinder.h"

//-----------------------------------------------------------------------------
// constants
//-----------------------------------------------------------------------------
const uInt2 c_iRadialTesselation = 32;
const uInt2 c_iRadialVertexCount = c_iRadialTesselation + 1;
const uInt2 c_iCylinderVertexCount = c_iRadialVertexCount * 2;
const uInt2 c_iCapVertexCount = c_iRadialVertexCount + 1;
const uInt2 c_iTotalVertexCount = c_iCylinderVertexCount + (2 * c_iCapVertexCount);
const uInt2 c_iCylinderFaceCount = c_iRadialTesselation * 2;

//-----------------------------------------------------------------------------
// static class members
//-----------------------------------------------------------------------------
DEFINE_FACTORY (CylinderA);

//-----------------------------------------------------------------------------
// class methods
//-----------------------------------------------------------------------------
void
CylinderA::Render (const Direct3D& pDirect3D, UINT iRenderPass) const
{
    // do the parental thang
    SceneObject::Render (pDirect3D, iRenderPass);
    
    switch (iRenderPass)
    {
        case AMBIENT_PASS:
        case LIGHTING_PASS:
            D3D_REQUIRE (pDirect3D->SetVertexDeclaration (m_pD3DVertexDecl));
            m_cylinder.Render (pDirect3D);
            break;

        default:
            break;
    }
/*
    // do the parental thang
    DrawnObject::Render (pDirect3D, iRenderPass);

    // check the render pass
    switch (iRenderPass)
    {
        case AMBIENT_PASS:
        case LIGHTING_PASS:
            // render the cylinder wall as a triangle strip and the caps as two triangle fans
            D3D_REQUIRE (pDirect3D->DrawPrimitive (D3DPT_TRIANGLESTRIP, 0, c_iCylinderFaceCount));
            D3D_REQUIRE (pDirect3D->DrawPrimitive (D3DPT_TRIANGLEFAN, c_iCylinderVertexCount, c_iRadialTesselation));
            D3D_REQUIRE (pDirect3D->DrawPrimitive (D3DPT_TRIANGLEFAN, c_iCylinderVertexCount + c_iCapVertexCount, c_iRadialTesselation));
            break;

        default:
            break;
    }
*/
}

//-----------------------------------------------------------------------------
/* void */
CylinderA::CylinderA (const PtrToXMLNode& pXMLNode) : 
    DrawnObject (pXMLNode, c_iTotalVertexCount),
    m_cylinder (pXMLNode)
{
}

//-----------------------------------------------------------------------------
/* void */
CylinderA::~CylinderA (void)
{
}

//-----------------------------------------------------------------------------
