//-----------------------------------------------------------------------------
// Copyright (C) 2013 Bretton Wade All Rights Reserved
//-----------------------------------------------------------------------------

#ifndef     _VECTOR2_H_
#define     _VECTOR2_H_

//-----------------------------------------------------------------------------
// axis values
//-----------------------------------------------------------------------------
enum
{
    X_AXIS =        0,
    Y_AXIS =        1
};

//-----------------------------------------------------------------------------
// macros
//-----------------------------------------------------------------------------
#define	dot			%

//-----------------------------------------------------------------------------
// classes
//-----------------------------------------------------------------------------
class Vector2
{
    public:
        float           x, y;
        
    public:    
        // constructors and destructor
        /* void */      Vector2 (void) {}
        /* void */      Vector2 (const Vector2& vector2);
        /* void */      Vector2 (float _x, float _y);
                            
        // operators
        Vector2&        operator = (const Vector2& vector2);
		float			operator [] (Word axis) const;
		Vector2			operator + (const Vector2& vector2) const;
		Vector2			operator - (const Vector2& vector2) const;
		float			operator % (const Vector2& vector2) const;
		Vector2			operator * (float scale) const;

        // normal operations
        void            Set (float _x, float _y);
        void            SetEmpty (void);
		float			LengthSq (void);
		float			Length (void);
};

//-----------------------------------------------------------------------------
// global operators
//-----------------------------------------------------------------------------
inline
Vector2	operator * (float scale, const Vector2& vector2) { return vector2 * scale; }

//-----------------------------------------------------------------------------
// inlines
//-----------------------------------------------------------------------------
#ifndef     _VECTOR2_INL_
#include    "vector2.inl"
#endif  //  _VECTOR2_INL_

//-----------------------------------------------------------------------------

#endif  //  _VECTOR2_H_
