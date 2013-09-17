#include "tgeom.h"
#include "triangulate.h"
#include "chain_list.h"

struct XZF32
{
    float x, z;
};

extern "C" BOOL _stdcall Triangulate(UINT32 uNumVerts, XZF32* pInVerts,
                                     UINT32* puNumOutVerts, XZF32** ppOutVerts,
                                     UINT32* puNumOutIdx, UINT16** ppOutIdx,
                                     FLOAT32 fMaxEdgeLength)
{
    *puNumOutVerts = 0;
    *ppOutVerts = NULL;
    *puNumOutIdx = 0;
    *ppOutIdx = NULL;
    
    TChainList<float> chainListIn;
    TChain<float> chainIn;
    TChain<float> chainOut;
    TriangleVec trianglesOut;

    for (UINT32 nInVert = 0; nInVert < uNumVerts; ++nInVert)
    {
        chainIn.push_back(TPoint2<float>(pInVerts[nInVert].x, pInVerts[nInVert].z));
    }

    if (chainIn.TestWindingOrder() == TURNS_RIGHT)
    {
        // Reverse winding order
        std::reverse(chainIn.begin (), chainIn.end());
    }

    chainListIn.push_back(chainIn);

    Triangulate (chainListIn, chainOut, trianglesOut);
    ClampEdgeLengths (chainOut, trianglesOut, fMaxEdgeLength);

    if (trianglesOut.size() == 0)
    {
        return (FALSE);
    }

    UINT32 uNumOutVerts = UINT32(chainOut.size());
    XZF32* pOutVerts = new XZF32[uNumOutVerts];
    if (!pOutVerts)
    {
        return (FALSE);
    }

    UINT32 uNumIdx = UINT32(trianglesOut.size() * 3);
    UINT16* pOutIdx = new UINT16[uNumIdx];
    if (!pOutIdx)
    {
        delete [] pOutVerts;
        return (FALSE);
    }

    for (UINT32 nVert = 0; nVert < uNumOutVerts; ++nVert)
    {
        pOutVerts[nVert].x = chainOut[nVert].x;
        pOutVerts[nVert].z = chainOut[nVert].y;
    }

    for (UINT32 nTri = 0; nTri < trianglesOut.size(); ++nTri)
    {
        pOutIdx[nTri * 3 + 0] = trianglesOut[nTri].a;
        pOutIdx[nTri * 3 + 1] = trianglesOut[nTri].b;
        pOutIdx[nTri * 3 + 2] = trianglesOut[nTri].c;
    }

    *puNumOutVerts = uNumOutVerts;
    *ppOutVerts    = pOutVerts;

    *puNumOutIdx   = uNumIdx;
    *ppOutIdx      = pOutIdx;

    return (TRUE);
}
