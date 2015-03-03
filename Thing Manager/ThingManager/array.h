//-----------------------------------------------------------------------------
// Copyright (C) 1997-2006 Bretton Wade
// All Rights Reserved
//-----------------------------------------------------------------------------

#ifndef     _ARRAY_H_
#define     _ARRAY_H_

//-----------------------------------------------------------------------------
// includes
//-----------------------------------------------------------------------------
#ifndef     _UPPER_HALF_H_
#include    "upper_half.h"
#endif  //  _UPPER_HALF_H_

//-----------------------------------------------------------------------------
// helper templates for doing conversions in template
//-----------------------------------------------------------------------------
template<class extractType, class fromType>
class ExtractType
{
    public:
        extractType     operator () (const fromType& from) const;
};

//-----------------------------------------------------------------------------
template<class extractType, class fromType>
inline
extractType
ExtractType<extractType, fromType>::operator () (const fromType& from) const
{ 
    return s_cast<extractType> (from); 
}

//-----------------------------------------------------------------------------
template<>
inline
float
ExtractType<float, UpperHalf>::operator () (const UpperHalf& from) const
{ 
    return from.GetValue (); 
}

//-----------------------------------------------------------------------------
// template functions
//-----------------------------------------------------------------------------
template<class arrayType, class compareType>
Word
FindInSortedArray (const arrayType* data, const compareType& value, Word hi, Word lo = 0)
{
    static ExtractType<compareType, arrayType>    extract;
    
    // execute a binary search on a sorted array
    while (lo < hi)
    {
        Word    mid = lo + ((hi - lo) >> 1);
        if (extract (data[mid]) < value)
            lo = mid + 1;
        else
            hi = mid;
    }
    
    // regardless of whether or not the search value was found, lo == hi, and 
    // that's where it should be. It's up to the caller to check this and
    // decide how to handle it. Note that lo could be equal to '0' or 'size'
    // indicating the search value is off the left or right side of the vector
    return lo;
}

//-----------------------------------------------------------------------------

#endif  //  _ARRAY_H_
