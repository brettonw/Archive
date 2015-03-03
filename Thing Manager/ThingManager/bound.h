//-----------------------------------------------------------------------------
// Copyright (C) 2006 Bretton Wade, All Rights Reserved
//-----------------------------------------------------------------------------

#ifndef     _BOUND_H_
#define     _BOUND_H_

//-----------------------------------------------------------------------------
// includes
//-----------------------------------------------------------------------------
#ifndef     _VECTOR2_H_
#include    "vector2.h"
#endif  //  _VECTOR2_H_

//-----------------------------------------------------------------------------
// bound values
//-----------------------------------------------------------------------------
        enum
        {
            LOWER_BOUND =   0,
            UPPER_BOUND =   1
        };

//-----------------------------------------------------------------------------
// classes
//-----------------------------------------------------------------------------
class Bound
{
    public:
        float           m_bounds[2][2];
        
    public:    
        // constructors and destructor
        /* void */      Bound (void) {}
        /* void */      Bound (const Bound& bound);
        /* void */      ~Bound (void) {}
                            
        // operators
        Bound&          operator = (const Bound& bound);
        const float*    operator [] (Word axis) const;
        float*          operator [] (Word axis);
        
        // normal operations
        void            Set (float left, float right, float bottom, float top);
        void            SetEmpty (void);
        float           GetSize (Word axis) const;
        bool            ContainsPtAxis (Word axis, const Vector2& pt) const;
        bool            ContainsPt (const Vector2& pt) const;
static  bool            OverlapAxis (Word axis, const Bound& a, const Bound& b);
static  bool            Overlap (const Bound& a, const Bound& b);
};

//-----------------------------------------------------------------------------
// inlines
//-----------------------------------------------------------------------------
#ifndef     _BOUND_INL_
#include    "bound.inl"
#endif  //  _BOUND_INL_

//-----------------------------------------------------------------------------

#endif  //  _BOUND_H_
