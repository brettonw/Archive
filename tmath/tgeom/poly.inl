//-----------------------------------------------------------------------------
// Copyright (C) 2002-2005 Bretton Wade, All rights reserved
//-----------------------------------------------------------------------------

#ifndef     _POLY_INL_
#define     _POLY_INL_

//-----------------------------------------------------------------------------
// includes
//-----------------------------------------------------------------------------
#ifndef     _POLY_H_
#include    "poly.h"
#endif  //  _POLY_H_

#ifndef     _CLIP_H_
#include    "clip.h"
#endif  //  _CLIP_H_

#ifndef     _TRIANGULATE_H_
#include    "triangulate.h"
#endif  //  _TRIANGULATE_H_

#ifndef     __lla_h
#include    "LLA.h"
#endif  //  __lla_h

//-----------------------------------------------------------------------------
// types
//-----------------------------------------------------------------------------
struct RuntimeVertex
{
    D3DXVECTOR3 pt;
    D3DXVECTOR2 uv;
};

//-----------------------------------------------------------------------------
// template methods
//-----------------------------------------------------------------------------
template<class Real>
/* void */
TPoly2<Real>::TPoly2 (const TPoly2<Real>& poly) :
    m_uvOrigin (0, 0)
{
    m_chainList.assign (poly.m_chainList.begin (), poly.m_chainList.end ());
}

//-----------------------------------------------------------------------------
template<class Real>
/* void */
TPoly2<Real>::TPoly2 (const TChain<Real>& chain) :
    m_uvOrigin (0, 0)
{
    m_chainList.push_back (chain);
}

//-----------------------------------------------------------------------------
template<class Real>
/* void */
TPoly2<Real>::TPoly2 (const TChainList<Real>& chainList) :
    m_uvOrigin (0, 0)
{
    m_chainList.assign (chainList.begin (), chainList.end ());
}

//-----------------------------------------------------------------------------
template<class Real>
bool
TPoly2<Real>::IsEmpty (void) const
{
    return (m_chainList.size () == 0);
}

//-----------------------------------------------------------------------------
template<class Real>
TBound2<Real>
TPoly2<Real>::GetBound (void) const
{
    return m_chainList.GetBound ();
}

//-----------------------------------------------------------------------------
template<class Real>
bool
TPoly2<Real>::ContainsPoint (const TPoint2<Real>& point) const
{
    return m_chainList.ContainsPoint (point);
}

//-----------------------------------------------------------------------------
template<class Real>
TChainList<Real>&
TPoly2<Real>::GetChains (void)
{
    return m_chainList;
}

//-----------------------------------------------------------------------------
template<class Real>
const TChainList<Real>&
TPoly2<Real>::GetChains (void) const
{
    return m_chainList;
}

//-----------------------------------------------------------------------------
template<class Real>
TPoly2<Real>
TPoly2<Real>::ClipToEdge (const TEdge2<Real>& edge) const
{
    TPoly2  result;
    ::ClipToLine (result.m_chainList, m_chainList, edge);
    return result;
}

//-----------------------------------------------------------------------------
template<class Real>
TPoly2<Real>
TPoly2<Real>::ClipToBound (const TBound2<Real>& bound) const
{
    TPoly2  result;
    ::ClipToBound (result.m_chainList, m_chainList, bound);
    return result;
}

//-----------------------------------------------------------------------------
template<class Real>
void
TPoly2<Real>::Reverse (void)
{
    m_chainList.ReverseChains ();
}

//-----------------------------------------------------------------------------
template<class Real>
TPoly2<Real>
TPoly2<Real>::Duplicate (void)
{
    TPoly2  result;
    result.m_chainList.Duplicate (m_chainList);
    return result;
}

//-----------------------------------------------------------------------------
template<class Real>
uInt
TPoly2<Real>::OutputTriangles (const TPoint2<Real>& ptOrigin, FILE* fp) const
{
    // triangulate the chain list
    TChain<Real>    outputChain;
    TriangleVec     outputTriangles;
    Triangulate (m_chainList, outputChain, outputTriangles);
    
    // write the file header
    uInt2           iVertexCount = outputChain.size ();
    fwrite (&iVertexCount, sizeof (iVertexCount), 1, fp);
    Assert (outputTriangles.size () < SHRT_MAX);
    uInt2           iTriangleCount = outputTriangles.size ();
    fwrite (&iTriangleCount, sizeof (iTriangleCount), 1, fp);

    // iterate over the output chain to output all the points in lat/lon
    {
        TChain<Real>::iterator  chainIter = outputChain.begin ();
        TChain<Real>::iterator  chainEnd = outputChain.end ();
        while (chainIter != chainEnd)
        {
            TPoint2<Real>&  point = (*chainIter++).pt;
            LLA             lla;
            lla.SetLatDegrees (point.y);
            lla.SetLonDegrees (point.x);
            lla.SetAltitude (0);
            fwrite (&lla, sizeof (lla), 1, fp);
        }
    }
    
    // iterate over the output chain to output all the points in vertex format
    {
        RuntimeVertex           rtVertex;
        TChain<Real>::iterator  chainIter = outputChain.begin ();
        TChain<Real>::iterator  chainEnd = outputChain.end ();
        while (chainIter != chainEnd)
        {
            // fetch the current vertex
            TVertex2<Real>& vertex = *chainIter++;
            
            // compute the output point
            TVector2<Real>  vec = vertex.pt - ptOrigin;
            rtVertex.pt = D3DXVECTOR3 (static_cast<float> (vec.x), static_cast<float> (vec.y), 0);
            
            // compute the output texture coordinate
            rtVertex.uv = D3DXVECTOR2 (static_cast<float> (vertex.uv.x), static_cast<float> (vertex.uv.y));
            fwrite (&rtVertex, sizeof (rtVertex), 1, fp);
        }
    }
    
    // iterate over the triangle vector to output all the triangles
    {
        TriangleVec::iterator   triangleIter = outputTriangles.begin ();
        TriangleVec::iterator   triangleEnd = outputTriangles.end ();
        while (triangleIter != triangleEnd)
        {
            Triangle&   triangle = *triangleIter++;
            Assert (triangle.a < iVertexCount);
            Assert (triangle.b < iVertexCount);
            Assert (triangle.c < iVertexCount);
            Triangle    triangleLHC (triangle.c, triangle.b, triangle.a);
            fwrite (&triangleLHC, sizeof (triangleLHC), 1, fp);
        }
    }
    
    return outputTriangles.size ();
}

//-----------------------------------------------------------------------------
template<class Real>
void
TPoly2<Real>::SetUVCoordinates (const TPoint2<Real>& uvOrigin, const TBound2<Real>& uvBound)
{
    // store the uv origin
    m_uvOrigin = uvOrigin;
    
    // propogate the set call
    m_chainList.SetUVCoordinates (uvBound);
}

//-----------------------------------------------------------------------------
template<class Real>
const TPoint2<Real>
TPoly2<Real>::GetUVOrigin (void) const
{
    return m_uvOrigin;
}

//-----------------------------------------------------------------------------
template<class Real>
void
TPoly2<Real>::Merge (TPoly2<Real>& dest, const TPoly2<Real>& src)
        
{
    dest.m_chainList.insert (dest.m_chainList.end (), src.m_chainList.begin (),src.m_chainList.end ());
}

//-----------------------------------------------------------------------------

#endif  //  _POLY_INL_
