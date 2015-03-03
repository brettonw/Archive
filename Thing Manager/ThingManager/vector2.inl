//-----------------------------------------------------------------------------
// Copyright (C) 2006 Bretton Wade, All Rights Reserved
//-----------------------------------------------------------------------------

#ifndef     _VECTOR2_INL_
#define     _VECTOR2_INL_

//-----------------------------------------------------------------------------
// includes
//-----------------------------------------------------------------------------
#ifndef     _VECTOR2_H_
#include    "vector2.h"
#endif  //  _VECTOR2_H_

//-----------------------------------------------------------------------------
// inlines
//-----------------------------------------------------------------------------
inline
/* void */      
Vector2::Vector2 (const Vector2& vector2)
{
	x = vector2.x;
	y = vector2.y;
}

//-----------------------------------------------------------------------------
inline
/* void */      
Vector2::Vector2 (float _x, float _y)
{
	x = _x;
	y = _y;
}

//-----------------------------------------------------------------------------
inline
Vector2&        
Vector2::operator = (const Vector2& vector2)
{
	x = vector2.x;
	y = vector2.y;
	return *this;
}

//-----------------------------------------------------------------------------
inline
float    
Vector2::operator [] (Word axis) const
{
	return *(&x + axis);
}

//-----------------------------------------------------------------------------
inline
Vector2            
Vector2::operator + (const Vector2& vector2) const
{
	return Vector2 (x + vector2.x, y + vector2.y);
}

//-----------------------------------------------------------------------------
inline
Vector2            
Vector2::operator - (const Vector2& vector2) const
{
	return Vector2 (x - vector2.x, y - vector2.y);
}

//-----------------------------------------------------------------------------
inline
float            
Vector2::operator % (const Vector2& vector2) const
{
	return (x * vector2.x) + (y * vector2.y);
}

//-----------------------------------------------------------------------------
inline
Vector2            
Vector2::operator * (float scale) const
{
	return Vector2 (x * scale, y * scale);
}

//-----------------------------------------------------------------------------
inline
void            
Vector2::Set (float _x, float _y)
{
	x = _x;
	y = _y;
}

//-----------------------------------------------------------------------------
inline
void            
Vector2::SetEmpty (void)
{
	x = 0;
	y = 0;
}

//-----------------------------------------------------------------------------
inline
float			
Vector2::LengthSq (void)
{
	return *this dot *this;
}

//-----------------------------------------------------------------------------
inline
float			
Vector2::Length (void)
{
	return sqrtf (LengthSq ());
}

//-----------------------------------------------------------------------------

#endif  //  _VECTOR2_INL_

