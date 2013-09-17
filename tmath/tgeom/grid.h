//-----------------------------------------------------------------------------
// Copyright (C) 2002-2005 Bretton Wade, All rights reserved
//-----------------------------------------------------------------------------

#ifndef     _GRID_H_
#define     _GRID_H_

//-----------------------------------------------------------------------------
// includes
//-----------------------------------------------------------------------------
#ifndef     _POLY_H_
#include    "poly.h"
#endif  //  _POLY_H_

//-----------------------------------------------------------------------------
// types
//-----------------------------------------------------------------------------
template<class Real>
class PolyMap : public stdext::hash_map<uInt, TPoly2<Real> > {};

//-----------------------------------------------------------------------------
// class definitions
//-----------------------------------------------------------------------------
template<class Real>
class Grid
{
    public:
        /* void */              Grid (Real fResolution, const TBound2<Real>& bound);
        
        void                    Insert (const TPoly2<Real>& poly);
        uInt                    GetCellCount (void) const;

        void                    OutputTriangles (FILE* fp) const;
        void                    OutputCells (const TPoint2<Real>& ptOrigin, const char* szDirectoryName) const;
        
        void                    SetUVCoordinates (void);
        
        const TPoly2<Real>*     GetCellPoly (const TPoint2<Real>& pointInCell);
        TPoly2<Real>            GetMergePoly (void) const;
    
    protected:
        struct CellIndex
        {
            uInt2   x;
            uInt2   y;
            CellIndex (uInt hash) : x ((hash >> 16) bit_and 0x0000ffff), y (hash bit_and 0x0000ffff) {}
            CellIndex (uInt2 _x, uInt2 _y) : x (_x), y (_y) {}
            operator uInt (void) const { return (x << 16) bit_or y; }
        };

        CellIndex               GetCellFromPoint (const TPoint2<Real>& point) const;
        TPoint2<Real>           GetCellOrigin (CellIndex index) const;
        TBound2<Real>           GetCellBound (CellIndex index) const;
    
    protected:
        Real                    m_rResolution;
        TBound2<Real>           m_bound;
        PolyMap<Real>           m_polyMap;
};

//-----------------------------------------------------------------------------
// inlines
//-----------------------------------------------------------------------------
#ifndef     _GRID_INL_
#include    "grid.inl"
#endif  //  _GRID_INL_

//-----------------------------------------------------------------------------

#endif  //  _GRID_H_
