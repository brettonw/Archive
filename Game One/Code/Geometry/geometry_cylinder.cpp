//-----------------------------------------------------------------------------
// Copyright (C) 1997-2006 Bretton Wade
// All Rights Reserved
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// include files
//-----------------------------------------------------------------------------
#include    "precompile.h"
#include    "geometry_cylinder.h"

//-----------------------------------------------------------------------------
// static class members
//-----------------------------------------------------------------------------
DEFINE_FACTORY (Cylinder);

//-----------------------------------------------------------------------------
// class methods
//-----------------------------------------------------------------------------
/* void */
Cylinder::Cylinder (const PtrToXMLNode& pXMLNode) : 
    GeometryObjectIndexed (pXMLNode)
{
    // this function computes the triangles of a cylinder with caps at the top 
    // and bottom. For texturing purposes, we assume the presence of a texture 
    // sheet that covers the cylinder wall in the range [0..1, 0..0.5], the top
    // in the range [0..0.5, 0.5..1], and the bottom in the range [0.5..1, 0.5..1]
    
    // fetch the tessellation value from the XML, default to 32
    uInt2           iRadialTesselation = 32;
    sscanf (pXMLNode->GetAttribute ("tessellation"), "%hu", &iRadialTesselation);
    uInt2           iRadialVertexCount = iRadialTesselation + 1;

    // compute the reference vectors
    D3DXVECTOR3     up (0.0f, 1.0f, 0.0f);
    D3DXVECTOR3     dn (0.0f, -1.0f, 0.0f);

    // compute the cap centers
    Vertex          upCenter (up, up, D3DXVECTOR2 (0.25f, 0.75f));
    Vertex          dnCenter (dn, dn, D3DXVECTOR2 (0.75f, 0.75f));
        
    // compute the 0% iteration in advance so the algorithm starts
    // with the history it needs to build wedges
    D3DXVECTOR3     vector (cosf (0.0f), 0.0f, sinf (0.0f));
    D3DXVECTOR2     uv = ((D3DXVECTOR2 (vector.x, vector.z) + D3DXVECTOR2 (1.0f, 1.0f)) * 0.25f) + D3DXVECTOR2 (0.0f, 0.5f);
    Vertex          upA (up + vector, up, uv);
    Vertex          dnA (dn + vector, dn, D3DXVECTOR2 (uv.x + 0.5f, uv.y));
    Vertex          wallA (up + vector, vector, D3DXVECTOR2 (0.0f, 0.0f));
    Vertex          wallB (dn + vector, vector, D3DXVECTOR2 (0.0f, 0.5f));
    
    // build the cylinder one wedge at a time
    for (uInt2 i = 1; i < iRadialVertexCount; i++)
    {
        // compute a fraction that describes how far around the cylinder we are
        float           fFraction = static_cast<float> (i) / static_cast<float> (iRadialTesselation);

        // generate a reference vector from the current angle
        float           fCurrentAngle = c_f2PI * fFraction;
        D3DXVECTOR3     vector (cosf (fCurrentAngle), 0, sinf (fCurrentAngle));
        D3DXVECTOR2     uv = ((D3DXVECTOR2 (vector.x, vector.z) + D3DXVECTOR2 (1.0f, 1.0f)) * 0.25f) + D3DXVECTOR2 (0.0f, 0.5f);
        
        // compute the next iteration
        Vertex          upB (up + vector, up, uv);
        Vertex          dnB (dn + vector, dn, D3DXVECTOR2 (uv.x + 0.5f, uv.y));
        Vertex          wallC (up + vector, vector, D3DXVECTOR2 (fFraction, 0.0f));
        Vertex          wallD (dn + vector, vector, D3DXVECTOR2 (fFraction, 0.5f));
        
        // emit 4 triangles for this wedge
        AddTriangle (upCenter, upB, upA);
        AddTriangle (dnCenter, dnA, dnB);
        AddTriangle (wallA, wallC, wallB);
        AddTriangle (wallC, wallD, wallB);
        
        // propogate the iteration to the history
        upA = upB;
        dnA = dnB;
        wallA = wallC;
        wallB = wallD;
    }
    
    // finish up...
    BuildObject ();
}

//-----------------------------------------------------------------------------
/* void */
Cylinder::~Cylinder (void)
{
}

//-----------------------------------------------------------------------------
