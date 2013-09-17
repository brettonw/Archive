//-----------------------------------------------------------------------------
// Copyright (C) 2003 Bretton Wade, All rights reserved
//-----------------------------------------------------------------------------

#ifndef     _META_H_
#define     _META_H_

//-----------------------------------------------------------------------------
// compiler control options
//-----------------------------------------------------------------------------
#pragma inline_recursion(on)
#pragma inline_depth(255)

//-----------------------------------------------------------------------------
// template meta classes with no static parameters
//-----------------------------------------------------------------------------
template<typename Number>
struct Meta0
{
    //-------------------------------------------------------------------------
    template<unsigned iCount>
    static inline
    Number*    
    Set (Number* pDst, Number value)
    {
        *pDst = value;
        return Set<iCount - 1> (pDst + 1, value);
    }
    
    //-------------------------------------------------------------------------
    template<>
    static inline
    Number*    
    Set<1> (Number* pDst, Number value)
    {
        *pDst = value;
        return pDst + 1;
    }
    
    //-------------------------------------------------------------------------
    template<unsigned iCount>
    static inline
    Number*    
    Copy (const Number* pSrc, Number* pDst)
    {
        *pDst = *pSrc;
        return Copy<iCount - 1> (pSrc + 1, pDst + 1);
    }
    
    //-------------------------------------------------------------------------
    template<>
    static inline
    Number*    
    Copy<1> (const Number* pSrc, Number* pDst)
    {
        *pDst = *pSrc;
        return pDst + 1;
    }
    
    //-------------------------------------------------------------------------
    template<unsigned iCount>
    static inline
    Number*    
    CopyAndNegate (const Number* pSrc, Number* pDst)
    {
        *pDst = -*pSrc;
        return CopyAndNegate<iCount - 1> (pSrc + 1, pDst + 1);
    }
    
    //-------------------------------------------------------------------------
    template<>
    static inline
    Number*    
    CopyAndNegate<1> (const Number* pSrc, Number* pDst)
    {
        *pDst = -*pSrc;
        return pDst + 1;
    }
    
    //-------------------------------------------------------------------------
    template<unsigned iCount>
    static inline
    Number     
    MultiplyAndAdd (const Number* pLHS, const Number* pRHS)
    {
        return (*pLHS * *pRHS) + MultiplyAndAdd<iCount - 1> (pLHS + 1, pRHS + 1);
    }
    
    //-------------------------------------------------------------------------
    template<>
    static inline
    Number     
    MultiplyAndAdd<1> (const Number* pLHS, const Number* pRHS) 
    {
        return (*pLHS * *pRHS);
    }
    
    //-------------------------------------------------------------------------
    template<unsigned iCount>
    static inline
    Number     
    SquareAndAdd (const Number* pSrc)
    {
        return (*pSrc * *pSrc) + SquareAndAdd<iCount - 1> (pSrc + 1);
    }
    
    //-------------------------------------------------------------------------
    template<>
    static inline
    Number     
    SquareAndAdd<1> (const Number* pSrc) 
    {
        return (*pSrc * *pSrc);
    }
    
    //-------------------------------------------------------------------------
    template<unsigned iCount>
    static inline
    void      
    Multiply (const Number* pLHS, const Number& scalar, Number* pDst)
    {
        *pDst = *pLHS * scalar;
        Multiply<iCount - 1> (pLHS + 1, scalar, pDst + 1);
    }
    
    //-------------------------------------------------------------------------
    template<>
    static inline
    void      
    Multiply<1> (const Number* pLHS, const Number& scalar, Number* pDst) 
    {
        *pDst = *pLHS * scalar;
    }
    
    //-------------------------------------------------------------------------
    template<unsigned iCount>
    static inline
    void      
    Divide (const Number* pLHS, const Number& scalar, Number* pDst)
    {
        *pDst = *pLHS / scalar;
        Divide<iCount - 1> (pLHS + 1, scalar, pDst + 1);
    }
    
    //-------------------------------------------------------------------------
    template<>
    static inline
    void      
    Divide<1> (const Number* pLHS, const Number& scalar, Number* pDst) 
    {
        *pDst = *pLHS / scalar;
    }
    
    //-------------------------------------------------------------------------
    template<unsigned iCount>
    static inline
    Number     
    Sum (const Number* pSrc)
    {
        return *pSrc + Sum<iCount - 1> (pSrc + 1);
    }
    
    //-------------------------------------------------------------------------
    template<>
    static inline
    Number     
    Sum<1> (const Number* pSrc)
    {
        return *pSrc;
    }
    
    //-------------------------------------------------------------------------
    template<unsigned iCount>
    static inline
    void      
    Add (const Number* pLHS, const Number* pRHS, Number* pDst)
    {
        *pDst = *pLHS + *pRHS;
        Add<iCount - 1> (pLHS + 1, pRHS + 1, pDst + 1);
    }
    
    //-------------------------------------------------------------------------
    template<>
    static inline
    void      
    Add<1> (const Number* pLHS, const Number* pRHS, Number* pDst)
    {
        *pDst = *pLHS + *pRHS;
    }
    
    //-------------------------------------------------------------------------
    template<unsigned iCount>
    static inline
    void      
    Sub (const Number* pLHS, const Number* pRHS, Number* pDst)
    {
        *pDst = *pLHS - *pRHS;
        Sub<iCount - 1> (pLHS + 1, pRHS + 1, pDst + 1);
    }
    
    //-------------------------------------------------------------------------
    template<>
    static inline
    void      
    Sub<1> (const Number* pLHS, const Number* pRHS, Number* pDst)
    {
        *pDst = *pLHS - *pRHS;
    }
    
    //-------------------------------------------------------------------------
    template<unsigned iCount>
    static inline
    bool      
    Equality (const Number* pLHS, const Number* pRHS)
    {
        return (*pLHS == *pRHS) and Equality<iCount - 1> (pLHS + 1, pRHS + 1);
    }
    
    //-------------------------------------------------------------------------
    template<>
    static inline
    bool      
    Equality<1> (const Number* pLHS, const Number* pRHS)
    {
        return (*pLHS == *pRHS);
    }
    
    //-------------------------------------------------------------------------
    template<unsigned iCount>
    static inline
    void      
    Print (std::ostream& stream, const Number* pSrc)
    {
        stream << *pSrc << ", ";
        Print<iCount - 1> (stream, pSrc + 1);
    }
    
    //-------------------------------------------------------------------------
    template<>
    static inline
    void      
    Print<1> (std::ostream& stream, const Number* pSrc)
    {
        stream << *pSrc;
    }
    
    //-------------------------------------------------------------------------
};

//-----------------------------------------------------------------------------
template<typename SourceType, typename DestinationType>
struct Meta0_Convert
{
    //-------------------------------------------------------------------------
    template<unsigned iCount>
    static inline
    DestinationType*  
    Copy (const SourceType* pSrc, DestinationType* pDst)
    {
        *pDst = static_cast<DestinationType> (*pSrc);
        return Copy<iCount - 1> (pSrc + 1, pDst + 1);
    }
    
    //-------------------------------------------------------------------------
    template<>
    static inline
    DestinationType*  
    Copy<1> (const SourceType* pSrc, DestinationType* pDst)
    {
        *pDst = static_cast<DestinationType> (*pSrc);
        return pDst + 1;
    }
    
    //-------------------------------------------------------------------------
};

//-----------------------------------------------------------------------------
// template meta classes with one static parameter
//-----------------------------------------------------------------------------
template<typename Number, unsigned iParam>
struct Meta1
{
    //-------------------------------------------------------------------------
    template<unsigned iCount>
    static inline
    Number     
    InnerProduct (const Number* a, const Number* b)
    {
        return (*a * *b) + InnerProduct<iCount - 1> (a + 1, b + iParam);
    }
    
    //-------------------------------------------------------------------------
    template<>
    static inline
    Number     
    InnerProduct<1> (const Number* a, const Number* b)
    {
        return (*a * *b);
    }
    
    //-------------------------------------------------------------------------
    template<unsigned iCount>
    static inline
    void      
    TransposeCopy (const Number* pSrc, Number* pDst)
    {
        *pDst = *pSrc;
        TransposeCopy<iCount - 1> (pSrc + 1, pDst + iParam);
    }
    
    //-------------------------------------------------------------------------
    template<>
    static inline
    void      
    TransposeCopy<1> (const Number* pSrc, Number* pDst)
    {
        *pDst = *pSrc;
    }
    
    //-------------------------------------------------------------------------
    template<unsigned iCount>
    static inline
    void      
    IdentityDiagonal (Number* pDst)
    {
        *pDst = 1;
        IdentityDiagonal<iCount - 1> (pDst + iParam + 1);
    }
    
    //-------------------------------------------------------------------------
    template<>
    static inline
    void      
    IdentityDiagonal<1> (Number* pDst)
    {
        *pDst = 1;
    }
    
    //-------------------------------------------------------------------------
};

//-----------------------------------------------------------------------------
template<typename Number, int hDefault, unsigned iParam>
struct Meta1_Homo
{
    //-------------------------------------------------------------------------
    template<unsigned iCount>
    static inline
    Number     
    InnerProduct (const Number* a, const Number* b)
    {
        return (*a * *b) + InnerProduct<iCount - 1> (a + 1, b + iParam);
    }
    
    //-------------------------------------------------------------------------
    template<>
    static inline
    Number     
    InnerProduct<1> (const Number* a, const Number* b)
    {
        return (Number (hDefault) * *b);
    }
    
    //-------------------------------------------------------------------------
};

//-----------------------------------------------------------------------------
// template meta classes with two static parameters
//-----------------------------------------------------------------------------
template<typename Number, unsigned iParam1, unsigned iParam2>
struct Meta2
{
    //-------------------------------------------------------------------------
    template<unsigned iCount>
    static inline
    Number*    
    RowOfInnerProducts (Number* pDst, const Number* pRow, const Number* pCol)
    {
        *pDst = Meta1<Number, iParam1>::InnerProduct<iParam2> (pRow, pCol);
        return RowOfInnerProducts<iCount - 1> (pDst + 1, pRow, pCol + 1);
    }
    
    //-------------------------------------------------------------------------
    template<>
    static inline
    Number*    
    RowOfInnerProducts<1> (Number* pDst, const Number* pRow, const Number* pCol)
    {
        *pDst = Meta1<Number, iParam1>::InnerProduct<iParam2> (pRow, pCol);
        return pDst + 1;
    }
    
    //-------------------------------------------------------------------------
    template<unsigned iCount>
    static inline
    void      
    MatMultOuterLoop (Number* pDst, const Number* pRow, const Number* pCol)
    {
        pDst = RowOfInnerProducts<iParam1> (pDst, pRow, pCol);
        MatMultOuterLoop<iCount - 1> (pDst, pRow + iParam2, pCol);
    }
    
    //-------------------------------------------------------------------------
    template<>
    static inline
    void      
    MatMultOuterLoop<1> (Number* pDst, const Number* pRow, const Number* pCol)
    {
        RowOfInnerProducts<iParam1> (pDst, pRow, pCol);
    }
    
    //-------------------------------------------------------------------------
    template<unsigned iCount>
    static inline
    void      
    TransposeOuterLoop (const Number* pSrc, Number* pDst)
    {
        Meta1<Number, iParam1>::TransposeCopy<iParam2> (pSrc, pDst);
        TransposeOuterLoop<iCount - 1> (pSrc + iParam2, pDst + 1);
    }
    
    //-------------------------------------------------------------------------
    template<>
    static inline
    void      
    TransposeOuterLoop<1> (const Number* pSrc, Number* pDst)
    {
        Meta1<Number, iParam1>::TransposeCopy<iParam2> (pSrc, pDst);
    }
    
    //-------------------------------------------------------------------------
};

//-----------------------------------------------------------------------------
template<typename Number, int hDefault, unsigned iParam1, unsigned iParam2>
struct Meta2_Homo
{
    //-------------------------------------------------------------------------
    template<unsigned iCount>
    static inline
    Number*    
    RowOfInnerProducts (Number* pDst, const Number* pRow, const Number* pCol)
    {
        *pDst = Meta1_Homo<Number, hDefault, iParam1>::InnerProduct<iParam2> (pRow, pCol);
        return RowOfInnerProducts<iCount - 1> (pDst + 1, pRow, pCol + 1);
    }
    
    //-------------------------------------------------------------------------
    template<>
    static inline
    Number*    
    RowOfInnerProducts<1> (Number* pDst, const Number* pRow, const Number* pCol)
    {
        *pDst = Meta1_Homo<Number, hDefault, iParam1>::InnerProduct<iParam2> (pRow, pCol);
        return pDst + 1;
    }
    
    //-------------------------------------------------------------------------
};

//-----------------------------------------------------------------------------

#endif  //  _META_H_
