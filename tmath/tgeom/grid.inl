//-----------------------------------------------------------------------------
// Copyright (C) 2002-2005 Bretton Wade, All rights reserved
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// includes
//-----------------------------------------------------------------------------
#ifndef     _GRID_H_
#include    "grid.h"
#endif  //  _GRID_H_

//-----------------------------------------------------------------------------
// template methods
//-----------------------------------------------------------------------------
template<class Real>
/* void */
Grid<Real>::Grid (Real rResolution, const TBound2<Real>& bound) :
    m_rResolution (rResolution),
    m_bound (bound)
{
}

//-----------------------------------------------------------------------------
template<class Real>
void
Grid<Real>::Insert (const TPoly2<Real>& inputPoly)
{
    // check to see if there is any work to do
    if (not inputPoly.IsEmpty ())
    {
        // fetch the chain list from the poly
        const TChainList<Real>&             chainList = inputPoly.GetChains ();
    
        // loop over all of the chains in the poly
        TChainList<Real>::const_iterator    chainIter = chainList.begin ();
        TChainList<Real>::const_iterator    chainEnd = chainList.end ();
        while (chainIter != chainEnd)
        {
            // fetch the current chain, and report it
            const TChain<Real>& chain = *chainIter++;

            // make a single chain poly from the chain, and get a bound on it
            TPoly2<Real>        poly (chain);
            TBound2<Real>       polyBound = poly.GetBound ();
            
            // convert the bound to a pair of cell indices
            CellIndex           minIndex = GetCellFromPoint (polyBound.GetMin ());
            CellIndex           maxIndex = GetCellFromPoint (polyBound.GetMax ());
            
            // loop over the cells covered by the bound
            for (uInt2 x = minIndex.x; x <= maxIndex.x; x++)
            {
                for (uInt2 y = minIndex.y; y <= maxIndex.y; y++)
                {
                    // build an index from the cell coordinates
                    CellIndex       cellIndex (x, y);
                    
                    // get a bound on the cell
                    TBound2<Real>   cellBound = GetCellBound (cellIndex);
                    
                    // clip the source poly to the cell
                    TPoly2<Real>    clippedPoly = poly.ClipToBound (cellBound);
                    
                    // check to see if the result has anything in it
                    if (not clippedPoly.IsEmpty ())
                    {
                        // fetch the actual cell contents
                        TPoly2<Real>&   cellPoly = m_polyMap[cellIndex];
                        
                        // merge the clipped poly into the cell poly
                        TPoly2<Real>::Merge (cellPoly, clippedPoly);
                    }
                }
            }
        }
    }
}

//-----------------------------------------------------------------------------
template<class Real>
uInt
Grid<Real>::GetCellCount (void) const
{
    return m_polyMap.size ();
}

//-----------------------------------------------------------------------------
template<class Real>
void
Grid<Real>::OutputTriangles (FILE* fp) const
{
    // set up an output poly
    TPoly2<Real>            outputPoly;
    
    // loop over all of the stored poly regions
    PolyMap::const_iterator iter = m_polyMap.begin ();
    PolyMap::const_iterator end = m_polyMap.end ();
    while (iter != end)
    {
        // fetch the current poly and advance the iterator
        const TPoly2<Real>&     sourcePoly = iter->second;
        ++iter;
        
        // merge the source poly into the output poly
        TPoly2::Merge (outputPoly, sourcePoly);
    }
    
    // output the output poly
    outputPoly.OutputTriangles (TPoint2<Real> (0, 0), fp);
}

//-----------------------------------------------------------------------------
template<class Real>
void
Grid<Real>::OutputCells (const TPoint2<Real>& ptOrigin, const char* szDirectoryName) const
{
    // open a file for the index, and check that it succeeds
    char                            szFileName[MAX_PATH];
    sprintf (szFileName, "%s\\cells.idx", szDirectoryName);
    FILE*                           fp = fopen (szFileName, "wb");
    if (fp)
    {
        // write our own header information
        fwrite (&m_bound, sizeof (m_bound), 1, fp);
        fwrite (&m_rResolution, sizeof (m_rResolution), 1, fp);
        uInt    iDirNameLen = strlen (szDirectoryName);
        fwrite (&iDirNameLen, sizeof (iDirNameLen), 1, fp);
        fwrite (szDirectoryName, sizeof (char), iDirNameLen, fp);
        
        // close the file
        fclose (fp);
    }
    
    // loop over all of the stored poly regions
    uInt                            iTotalTriangles = 0;
    uInt                            iMostTriangles = 0;
    PolyMap<Real>::const_iterator   iter = m_polyMap.begin ();
    PolyMap<Real>::const_iterator   end = m_polyMap.end ();
    while (iter != end)
    {
        // fetch the current poly and advance the iterator
        const TPoly2<Real>& sourcePoly = iter->second;
        
        // figure out the filename
        CellIndex           index (iter->first);
        sprintf (szFileName, "%s\\%s%05dx%05d.cel", szDirectoryName, szDirectoryName, index.x, index.y);
        
        // open a file, and check to see if it succeeds
        fp = fopen (szFileName, "wb");
        if (fp)
        {
            // get the cell bound, and compute the uv origin
            TPoint2<float>  uvOrigin (sourcePoly.GetUVOrigin ());
        
            // write the uv origin for the cell
            fwrite (&uvOrigin, sizeof (uvOrigin), 1, fp);
            
            // get the origin for the cell, and output the poly it contains
            uInt            iTriangleCount = sourcePoly.OutputTriangles (ptOrigin, fp);
            iTotalTriangles += iTriangleCount;
            iMostTriangles = (iMostTriangles > iTriangleCount) ? iMostTriangles : iTriangleCount;
            
            // close the file
            fclose (fp);
        }
        
        // advance the iterator
        ++iter;
    }
    
    // report some statistics
    printf ("Average triangle count per occupied grid cell: %g\n", static_cast<double> (iTotalTriangles) / static_cast<double> (m_polyMap.size ()));
    printf ("Largest triangle count in an occupied grid cell: %u\n", iMostTriangles);
    TVector2<Real>                  vSpan = (m_bound.GetMax () - m_bound.GetMin ()) / m_rResolution;
    printf ("Occupancy: %g\n", static_cast<double> (m_polyMap.size ()) / static_cast<double>(vSpan.x * vSpan.y));
}

//-----------------------------------------------------------------------------
template<class Real>
void
Grid<Real>::SetUVCoordinates (void)
{
    // compute the span of the grid bound
    TVector2<Real>                  vSpan = m_bound.GetMax () - m_bound.GetMin ();
    
    // loop over all of the stored poly regions
    uInt                            iTotalTriangles = 0;
    uInt                            iMostTriangles = 0;
    PolyMap<Real>::iterator         iter = m_polyMap.begin ();
    PolyMap<Real>::iterator         end = m_polyMap.end ();
    while (iter != end)
    {
        // get the cell index and the cell bound from that
        CellIndex       index (iter->first);
        TBound2<Real>   uvBound = GetCellBound (index);
        
        // compute the uv origin
        const TVector2<Real>&   vLowerLeft = uvBound.GetMin () - m_bound.GetMin ();
        TPoint2<Real>           uvOrigin (vLowerLeft.x / vSpan.x, vLowerLeft.y / vSpan.y);
        
        // fetch the current poly and advance the iterator
        TPoly2<Real>&   sourcePoly = iter->second;
        
        // set the uv coordinates on the poly based on the cell bound
        sourcePoly.SetUVCoordinates (uvOrigin, uvBound);
        
        // advance the iterator
        ++iter;
    }
}

//-----------------------------------------------------------------------------
template<class Real>
const TPoly2<Real>*
Grid<Real>::GetCellPoly (const TPoint2<Real>& pointInCell)
{
    // get the cell index from the location
    Grid<Real>::CellIndex   cellIndex = GetCellFromPoint (pointInCell);
    
    // find the cell, and return a pointer to it if it was found
    PolyMap<Real>::iterator polyIter = m_polyMap.find (cellIndex);
    if (polyIter != m_polyMap.end ())
        return &(polyIter->second);
    
    // no poly found
    return 0;
}
    
//-----------------------------------------------------------------------------
template<class Real>
TPoly2<Real>
Grid<Real>::GetMergePoly (void) const
{
    // set up the poly to merge into
    TPoly2<Real>                    mergePoly;
    
    // loop over all of the stored poly regions
    PolyMap<Real>::const_iterator   iter = m_polyMap.begin ();
    PolyMap<Real>::const_iterator   end = m_polyMap.end ();
    while (iter != end)
    {
        // fetch the current poly and advance the iterator
        const TPoly2<Real>& sourcePoly = iter->second;
        
        // merge the current poly into the merge poly
        TPoly2<Real>::Merge (mergePoly, sourcePoly);
        
        // advance the iterator
        ++iter;
    }
    
    // return the merged poly
    return mergePoly;
}

//-----------------------------------------------------------------------------
template<class Real>
Grid<Real>::CellIndex
Grid<Real>::GetCellFromPoint (const TPoint2<Real>& point) const
{
    // subtract the grid origin
    TVector2<Real>  vDelta = point - m_bound.GetMin ();
    
    // divide the result by the resolution of the grid
    vDelta /= m_rResolution;
    
    // clamp the floating point coordinates to integers, and return the index
    return CellIndex (static_cast<uInt2> (vDelta.x), static_cast<uInt2> (vDelta.y));
}

//-----------------------------------------------------------------------------
template<class Real>
TPoint2<Real>
Grid<Real>::GetCellOrigin (Grid<Real>::CellIndex index) const
{
    // make a point from the index values
    TPoint2<Real>   ptMin (static_cast<Real> (index.x), static_cast<Real> (index.y));
    
    // scale the point by the resolution, add in the lower bound point, and
    // return the result
    return (TVector2<Real> (ptMin) * m_rResolution) + m_bound.GetMin ();
}

//-----------------------------------------------------------------------------
template<class Real>
TBound2<Real>
Grid<Real>::GetCellBound (Grid<Real>::CellIndex index) const
{
    // compute the bound points in world coordinates
    TPoint2<Real>   ptMin = GetCellOrigin (index);
    TPoint2<Real>   ptMax = ptMin + TVector2<Real> (m_rResolution, m_rResolution);
    
    // build a bound and return it
    return TBound2<Real> (ptMin, ptMax);
}

//-----------------------------------------------------------------------------
    
