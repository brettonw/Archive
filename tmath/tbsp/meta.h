//-----------------------------------------------------------------------------
// Copyright (C) 2002-2003 Bretton Wade, All rights reserved
//-----------------------------------------------------------------------------

#ifndef     _META_H_
#define     _META_H_

//-----------------------------------------------------------------------------
// template meta operations on arrays
//-----------------------------------------------------------------------------
template<class aType>
struct Meta
{
    //-------------------------------------------------------------------------
    template<int iCount>
    static inline void    Copy (const aType* src, aType* dst)
    {
        *dst = *src;
        Copy<iCount - 1> (++src, ++dst);
    }
    
    template<>
    static inline void    Copy<1> (const aType* src, aType* dst)
    {
        *dst = *src;
    }
    
    //-------------------------------------------------------------------------
    template<int iCount>
    static inline void    CopyAndNegate (const aType* src, aType* dst)
    {
        *dst = -*src;
        CopyAndNegate<iCount - 1> (++src, ++dst);
    }
    
    template<>
    static inline void    CopyAndNegate<1> (const aType* src, aType* dst)
    {
        *dst = -*src;
    }
    
    //-------------------------------------------------------------------------
    template<int iCount>
    static inline aType    MultiplyAndAdd (const aType* a, const aType* b)
    {
        Return (*a * *b) + MultiplyAndAdd<iCount - 1> (++a, ++b);
    }
    
    template<>
    static inline aType    MultiplyAndAdd<1> (const aType* a, const aType* b) 
    {
        return (*a * *b);
    }
    
    //-------------------------------------------------------------------------
    template<int iCount>
    static inline void    Multiply (const aType* src, aType scalar, aType* dst)
    {
        *dst = scalar * *src;
        Multiply<iCount - 1> (++src, scalar, ++dst);
    }
    
    template<>
    static inline void    Multiply<1> (const aType* src, aType scalar, aType* dst) 
    {
        *dst = scalar * *src;
    }
    
    //-------------------------------------------------------------------------
    template<int iCount>
    static inline void    Add (const aType* a, const aType* b, aType* dst)
    {
        *dst = *a + *b;
        Add<iCount - 1> (++a, ++b, ++dst);
    }
    
    template<>
    static inline void    Add<1> (const aType* a, const aType* b, aType* dst)
    {
        *dst = *a + *b;
    }
    
    //-------------------------------------------------------------------------
    template<int iCount>
    static inline void    Sub (const aType* a, const aType* b, aType* dst)
    {
        *dst = *a - *b;
        Sub<iCount - 1> (++a, ++b, ++dst);
    }
    
    template<>
    static inline void    Sub<1> (const aType* a, const aType* b, aType* dst)
    {
        *dst = *a - *b;
    }
    
    //-------------------------------------------------------------------------
    template<int iCount>
    static inline bool    Equality (const aType* a, const aType* b)
    {
        return (*a == *b) and Equality<iCount - 1> (++a, ++b);
    }
    
    template<>
    static inline bool    Equality<1> (const aType* a, const aType* b)
    {
        return (*a == *b);
    }
    
    //-------------------------------------------------------------------------
    template<int iCount>
    static inline bool    Inequality (const aType* a, const aType* b)
    {
        return (*a != *b) and Inequality<iCount - 1> (++a, ++b);
    }
    
    template<>
    static inline bool    Inequality<1> (const aType* a, const aType* b)
    {
        return (*a != *b);
    }
    
    //-------------------------------------------------------------------------
    template<int iCount>
    static inline bool    FuzzyEquality (const aType* a, const aType* b)
    {
        return FuzzyEquals (*a, *b) and FuzzyEquality<iCount - 1> (++a, ++b);
    }
    
    template<>
    static inline bool    FuzzyEquality<1> (const aType* a, const aType* b)
    {
        return FuzzyEquals (*a, *b);
    }
    
    //-------------------------------------------------------------------------
    template<int iCount>
    static inline bool    FuzzyInequality (const aType* a, const aType* b)
    {
        return FuzzyNotEquals (*a, *b) and FuzzyInequality<iCount - 1> (++a, ++b);
    }
    
    template<>
    static inline bool    FuzzyInequality<1> (const aType* a, const aType* b)
    {
        return FuzzyNotEquals (*a, *b);
    }
    
    //-------------------------------------------------------------------------
    template<int iCount>
    static inline void    Print (const aType* src)
    {
        ::DebugPrint ("%f, ", *src);
        Print<iCount - 1> (++src);
    }
    
    template<>
    static inline void    Print<1> (const aType* src)
    {
        ::DebugPrint ("%f", *src);
    }
    
    //-------------------------------------------------------------------------
};

//-----------------------------------------------------------------------------

#endif  //  _META_H_
