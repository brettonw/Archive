//-----------------------------------------------------------------------------
// Copyright (C) 2003 Bretton Wade, All rights reserved
//-----------------------------------------------------------------------------

#ifndef     _REAL_H_
#define     _REAL_H_

//-----------------------------------------------------------------------------
// namespace
//-----------------------------------------------------------------------------
BEGIN_NAMESPACE (TMath)

//-----------------------------------------------------------------------------
// template types for objects based on the matrix type
//-----------------------------------------------------------------------------
template<typename Number>
struct TReal
{
        /* void */                  TReal (void) {}
explicit/* void */                  TReal (const Number& number);
        /* void */                  TReal (const TReal<Number>& real);

        TReal<Number>&              operator = (const Number& number);
        TReal<Number>&              operator = (const TReal<Number>& real);

        bool                        operator == (const Number& number) const;
        bool                        operator != (const Number& number) const;
        bool                        operator >  (const Number& number) const;
        bool                        operator >= (const Number& number) const;
        bool                        operator <  (const Number& number) const;
        bool                        operator <= (const Number& number) const;

        bool                        operator == (const TReal<Number>& real) const;
        bool                        operator != (const TReal<Number>& real) const;
        bool                        operator >  (const TReal<Number>& real) const;
        bool                        operator >= (const TReal<Number>& real) const;
        bool                        operator <  (const TReal<Number>& real) const;
        bool                        operator <= (const TReal<Number>& real) const;
        
        TReal<Number>               operator - (void) const;

        TReal<Number>               operator + (const Number& number) const;
        TReal<Number>               operator - (const Number& number) const;
        TReal<Number>               operator * (const Number& number) const;
        TReal<Number>               operator / (const Number& number) const;
        TReal<Number>               operator % (const Number& number) const;

        TReal<Number>               operator + (const TReal<Number>& real) const;
        TReal<Number>               operator - (const TReal<Number>& real) const;
        TReal<Number>               operator * (const TReal<Number>& real) const;
        TReal<Number>               operator / (const TReal<Number>& real) const;
        TReal<Number>               operator % (const TReal<Number>& real) const;

        TReal<Number>&              operator += (const Number& number);
        TReal<Number>&              operator -= (const Number& number);
        TReal<Number>&              operator *= (const Number& number);
        TReal<Number>&              operator /= (const Number& number);
        TReal<Number>&              operator %= (const Number& number);

        TReal<Number>&              operator += (const TReal<Number>& real);
        TReal<Number>&              operator -= (const TReal<Number>& real);
        TReal<Number>&              operator *= (const TReal<Number>& real);
        TReal<Number>&              operator /= (const TReal<Number>& real);
        TReal<Number>&              operator %= (const TReal<Number>& real);
        
        #if 0
        /* const Number& */         operator const Number& (void) const;
        /* Number& */               operator Number& (void);
        #endif
        const Number&               operator () (void) const;
        Number&                     operator () (void);
        
static  void                        ComputeInfinity (void);
static  void                        SetInfinity (const TReal<Number>& infinity);
static  const TReal<Number>&        GetInfinity (void);
        
static  void                        ComputeEpsilon (void);
static  void                        SetEpsilon (const TReal<Number>& epsilon);
static  const TReal<Number>&        GetEpsilon (void);

static  void                        CheckValid (TReal<Number>& real, bool bCheckClass = true, bool bRound = false);
        
    private:
        Number                      m_number;
static  Number                      s_infinity;
static  Number                      s_epsilon;
};

//-----------------------------------------------------------------------------
// global operators
//-----------------------------------------------------------------------------
template<typename Number>
bool
operator == (const Number& lhs, const TReal<Number>& rhs);
    
template<typename Number>
bool
operator != (const Number& lhs, const TReal<Number>& rhs);
    
template<typename Number>
bool
operator >  (const Number& lhs, const TReal<Number>& rhs);
    
template<typename Number>
bool
operator >= (const Number& lhs, const TReal<Number>& rhs);
    
template<typename Number>
bool
operator <  (const Number& lhs, const TReal<Number>& rhs);
    
template<typename Number>
bool
operator <= (const Number& lhs, const TReal<Number>& rhs);
    
template<typename Number>
TReal<Number>
operator + (const Number& lhs, const TReal<Number>& rhs);
    
template<typename Number>
TReal<Number>
operator - (const Number& lhs, const TReal<Number>& rhs);
    
template<typename Number>
TReal<Number>
operator * (const Number& lhs, const TReal<Number>& rhs);
    
template<typename Number>
TReal<Number>
operator / (const Number& lhs, const TReal<Number>& rhs);
    
template<typename Number>
TReal<Number>
operator % (const Number& lhs, const TReal<Number>& rhs);
    
//-----------------------------------------------------------------------------
// stream operator for real types
//-----------------------------------------------------------------------------
template<typename Number>
std::ostream& 
operator << (std::ostream& stream, const TReal<Number>& real);

//-----------------------------------------------------------------------------
// typenames for export
//-----------------------------------------------------------------------------
typedef TReal<float>        Real32;
typedef TReal<double>       Real64;

//-----------------------------------------------------------------------------
// inlines
//-----------------------------------------------------------------------------
#ifndef     _REAL_INL_
#include    "real.inl"
#endif  //  _REAL_INL_

#ifndef     _REAL_MATH_INL_
#include    "real_math.inl"
#endif  //  _REAL_MATH_INL_

//-----------------------------------------------------------------------------
// namespace
//-----------------------------------------------------------------------------
END_NAMESPACE (TMath)

//-----------------------------------------------------------------------------

#endif  //  _REAL_H_
