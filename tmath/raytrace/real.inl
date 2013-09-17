//-----------------------------------------------------------------------------
// Copyright (C) 2003 Bretton Wade, All rights reserved
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// includes
//-----------------------------------------------------------------------------
#ifndef     _REAL_H_
#include    "real.h"
#endif      _REAL_H_

//-----------------------------------------------------------------------------
#ifndef     _REAL_INL_
#define     _REAL_INL_

//-----------------------------------------------------------------------------
// includes
//-----------------------------------------------------------------------------
#ifndef     _BASIC_MATH_H_
#include    "basic_math.h"
#endif      _BASIC_MATH_H_

//-----------------------------------------------------------------------------
// template static members for the real type
//-----------------------------------------------------------------------------
template<typename Number>
Number      TReal<Number>::s_infinity = Number (0);

template<typename Number>
Number      TReal<Number>::s_epsilon = Number (0);

//-----------------------------------------------------------------------------
// template member definitions for the basic matrix initializer type
//-----------------------------------------------------------------------------
template<typename Number>
/* void */
TReal<Number>::TReal (const Number& number) :
    m_number (number)
{
    CheckValid (*this);
}
    
//-----------------------------------------------------------------------------
template<typename Number>
/* void */
TReal<Number>::TReal (const TReal<Number>& real) :
    m_number (real.m_number)
{
    CheckValid (*this);
}

//-----------------------------------------------------------------------------
template<typename Number>
TReal<Number>&
TReal<Number>::operator = (const Number& number)
{
    m_number = number;
    CheckValid (*this);
    return *this;
}
    
//-----------------------------------------------------------------------------
template<typename Number>
TReal<Number>&
TReal<Number>::operator = (const TReal<Number>& real)
{
    m_number = real.m_number;
    CheckValid (*this);
    return *this;
}

//-----------------------------------------------------------------------------
template<typename Number>
bool
TReal<Number>::operator == (const Number& number) const
{
    return (AbsoluteValue (m_number - number) <= s_epsilon);
}
    
//-----------------------------------------------------------------------------
template<typename Number>
bool
TReal<Number>::operator != (const Number& number) const
{
    return (AbsoluteValue (m_number - number) > s_epsilon);
}
    
//-----------------------------------------------------------------------------
template<typename Number>
bool
TReal<Number>::operator >  (const Number& number) const
{
    return (m_number > number);
}
    
//-----------------------------------------------------------------------------
template<typename Number>
bool
TReal<Number>::operator >= (const Number& number) const
{
    return (m_number > (number - s_epsilon));
}
    
//-----------------------------------------------------------------------------
template<typename Number>
bool
TReal<Number>::operator <  (const Number& number) const
{
    return (m_number < number);
}
    
//-----------------------------------------------------------------------------
template<typename Number>
bool
TReal<Number>::operator <= (const Number& number) const
{
    return (m_number < (number + s_epsilon));
}
    
//-----------------------------------------------------------------------------
template<typename Number>
bool
TReal<Number>::operator == (const TReal<Number>& real) const
{
    return (AbsoluteValue (m_number - real.m_number) <= s_epsilon);
}
    
//-----------------------------------------------------------------------------
template<typename Number>
bool
TReal<Number>::operator != (const TReal<Number>& real) const
{
    return (AbsoluteValue (m_number - real.m_number) > s_epsilon);
}
    
//-----------------------------------------------------------------------------
template<typename Number>
bool
TReal<Number>::operator >  (const TReal<Number>& real) const
{
    return (m_number > real.m_number);
}
    
//-----------------------------------------------------------------------------
template<typename Number>
bool
TReal<Number>::operator >= (const TReal<Number>& real) const
{
    return (m_number > (real.m_number - s_epsilon));
}
    
//-----------------------------------------------------------------------------
template<typename Number>
bool
TReal<Number>::operator <  (const TReal<Number>& real) const
{
    return (m_number < real.m_number);
}
    
//-----------------------------------------------------------------------------
template<typename Number>
bool
TReal<Number>::operator <= (const TReal<Number>& real) const
{
    return (m_number < (real.m_number + s_epsilon));
}
    
//-----------------------------------------------------------------------------
template<typename Number>
TReal<Number>
TReal<Number>::operator - (void) const
{
    return -m_number;
}
    
//-----------------------------------------------------------------------------
template<typename Number>
TReal<Number>
TReal<Number>::operator + (const Number& number) const
{
    return m_number + number;
}
    
//-----------------------------------------------------------------------------
template<typename Number>
TReal<Number>
TReal<Number>::operator - (const Number& number) const
{
    return m_number - number;
}
    
//-----------------------------------------------------------------------------
template<typename Number>
TReal<Number>
TReal<Number>::operator * (const Number& number) const
{
    return m_number * number;
}
    
//-----------------------------------------------------------------------------
template<typename Number>
TReal<Number>
TReal<Number>::operator / (const Number& number) const
{
    return m_number / number;
}
    
//-----------------------------------------------------------------------------
template<typename Number>
TReal<Number>
TReal<Number>::operator % (const Number& number) const
{
    return Modulo (m_number, number);
}
    
//-----------------------------------------------------------------------------
template<typename Number>
TReal<Number>
TReal<Number>::operator + (const TReal<Number>& real) const
{
    return m_number + real.m_number;
}
    
//-----------------------------------------------------------------------------
template<typename Number>
TReal<Number>
TReal<Number>::operator - (const TReal<Number>& real) const
{
    return m_number - real.m_number;
}
    
//-----------------------------------------------------------------------------
template<typename Number>
TReal<Number>
TReal<Number>::operator * (const TReal<Number>& real) const
{
    return m_number * real.m_number;
}
    
//-----------------------------------------------------------------------------
template<typename Number>
TReal<Number>
TReal<Number>::operator / (const TReal<Number>& real) const
{
    return m_number / real.m_number;
}
    
//-----------------------------------------------------------------------------
template<typename Number>
TReal<Number>
TReal<Number>::operator % (const TReal<Number>& real) const
{
    return Modulo (m_number, number);
}
    
//-----------------------------------------------------------------------------
template<typename Number>
TReal<Number>&
TReal<Number>::operator += (const Number& number)
{
    m_number += number;
    CheckValid (*this);
    return *this;
}
    
//-----------------------------------------------------------------------------
template<typename Number>
TReal<Number>&
TReal<Number>::operator -= (const Number& number)
{
    m_number -= number;
    CheckValid (*this);
    return *this;
}
    
//-----------------------------------------------------------------------------
template<typename Number>
TReal<Number>&
TReal<Number>::operator *= (const Number& number)
{
    m_number *= number;
    CheckValid (*this);
    return *this;
}
    
//-----------------------------------------------------------------------------
template<typename Number>
TReal<Number>&
TReal<Number>::operator /= (const Number& number)
{
    m_number /= number;
    CheckValid (*this);
    return *this;
}
    
//-----------------------------------------------------------------------------
template<typename Number>
TReal<Number>&
TReal<Number>::operator %= (const Number& number)
{
    m_number  = Modulo (m_number, number);
    CheckValid (*this);
    return *this;
}
    
//-----------------------------------------------------------------------------
template<typename Number>
TReal<Number>&
TReal<Number>::operator += (const TReal<Number>& real)
{
    m_number += real.m_number;
    CheckValid (*this);
    return *this;
}
    
//-----------------------------------------------------------------------------
template<typename Number>
TReal<Number>&
TReal<Number>::operator -= (const TReal<Number>& real)
{
    m_number -= real.m_number;
    CheckValid (*this);
    return *this;
}
    
//-----------------------------------------------------------------------------
template<typename Number>
TReal<Number>& 
TReal<Number>::operator *= (const TReal<Number>& real)
{
    m_number *= real.m_number;
    CheckValid (*this);
    return *this;
}
    
//-----------------------------------------------------------------------------
template<typename Number>
TReal<Number>&
TReal<Number>::operator /= (const TReal<Number>& real)
{
    m_number /= real.m_number;
    CheckValid (*this);
    return *this;
}
    
//-----------------------------------------------------------------------------
template<typename Number>
TReal<Number>&
TReal<Number>::operator %= (const TReal<Number>& real)
{
    m_number = Modulo (m_number, real.m_number);
    CheckValid (*this);
    return *this;
}
    
//-----------------------------------------------------------------------------
#if 0
template<typename Number>
/* const Number& */
TReal<Number>::operator const Number& (void) const
{
    return m_number;
}
    
//-----------------------------------------------------------------------------
template<typename Number>
/* Number& */
TReal<Number>::operator Number& (void)
{
    return m_number;
}
#endif
//-----------------------------------------------------------------------------
template<typename Number>
const Number&
TReal<Number>::operator () (void) const
{
    return m_number;
}
    
//-----------------------------------------------------------------------------
template<typename Number>
Number&
TReal<Number>::operator () (void)
{
    return m_number;
}

//-----------------------------------------------------------------------------
template<typename Number>
void
TReal<Number>::ComputeInfinity (void)
{
    s_infinity = std::numeric_limits<Number>::infinity ();
    g_dbgout << "Infinity is: " << s_infinity << std::endl;
}
    
//-----------------------------------------------------------------------------
template<typename Number>
void
TReal<Number>::SetInfinity (const TReal<Number>& infinity)
{
    s_infinity = infinity.m_number;
}
    
//-----------------------------------------------------------------------------
template<typename Number>
const TReal<Number>&
TReal<Number>::GetInfinity (void)
{
    return s_infinity;
}
    
//-----------------------------------------------------------------------------
template<typename Number>
void
TReal<Number>::ComputeEpsilon (void)
{
    s_epsilon = Power (Number (10), Number (-std::numeric_limits<Number>::digits10));
    g_dbgout << "Epsilon is: " << s_epsilon << std::endl;
}
    
//-----------------------------------------------------------------------------
template<typename Number>
void
TReal<Number>::SetEpsilon (const TReal<Number>& epsilon)
{
    s_epsilon = epsilon.m_number;
}
    
//-----------------------------------------------------------------------------
template<typename Number>
const TReal<Number>&
TReal<Number>::GetEpsilon (void)
{
    return s_epsilon;
}
    
//-----------------------------------------------------------------------------
template<typename Number>
void
TReal<Number>::CheckValid (TReal<Number>& real, bool bCheckClass, bool bRound)
{
    #ifdef  _VALIDATE_REAL
    if (bCheckClass)
    {
        switch (_fpclass (real.m_number))
        {
            case _FPCLASS_QNAN:     // Quiet NaN 
            case _FPCLASS_SNAN:     // Signaling NaN 
                // not a number, possibly one of the following occurred:
                //    0 / 0
                //    infinity - infinity
                //    infinity / infinity
                //    infinity * 0
                g_dbgout << std::endl << "FP Class: NaN" << std::endl;
                break;
            case _FPCLASS_PINF:     // Positive infinity (+INF) 
            case _FPCLASS_NINF:     // Negative infinity ( –INF) 
                // infinite values, either explicit or the result of divide by 0
                g_dbgout << std::endl << "FP Class: Inf" << std::endl;
                break;
            case _FPCLASS_NN:       // Negative normalized non-zero 
            case _FPCLASS_PN:       // Positive normalized non-zero 
                // these are normal
                break;
            case _FPCLASS_NZ:       // Negative zero ( – 0) 
                //g_dbgout << std::endl << "FP Class: -0" << std::endl;
            case _FPCLASS_PZ:       // Positive 0 (+0) 
                // zero, fairly normal
                break;
            case _FPCLASS_PD:       // Positive denormalized 
            case _FPCLASS_ND:       // Negative denormalized 
                // this shouldn't happen
                g_dbgout << std::endl << "FP Class: Denormalized ???" << std::endl;
                break;
        }
    }
    
    if (bRound)
    {
        Number  round = RoundOff (real.m_number);
        if ((real.m_number != round) and (real == round))
        {
            //g_dbgout << std::endl << "Rounding " << real.m_number << " to " << round << std::endl;
            real.m_number = round;
        }
    }
    #endif
}
    
//-----------------------------------------------------------------------------
// global operators
//-----------------------------------------------------------------------------
template<typename Number>
bool
operator == (const Number& lhs, const TReal<Number>& rhs)
{
    return (rhs == lhs);
}
    
//-----------------------------------------------------------------------------
template<typename Number>
bool
operator != (const Number& lhs, const TReal<Number>& rhs)
{
    return (rhs != lhs);
}
    
//-----------------------------------------------------------------------------
template<typename Number>
bool
operator >  (const Number& lhs, const TReal<Number>& rhs)
{
    return (rhs <= lhs);
}
    
//-----------------------------------------------------------------------------
template<typename Number>
bool
operator >= (const Number& lhs, const TReal<Number>& rhs)
{
    return (rhs < lhs);
}
    
//-----------------------------------------------------------------------------
template<typename Number>
bool
operator <  (const Number& lhs, const TReal<Number>& rhs)
{
    return (rhs >= lhs);
}
    
//-----------------------------------------------------------------------------
template<typename Number>
bool
operator <= (const Number& lhs, const TReal<Number>& rhs)
{
    return (rhs > lhs);
}
    
//-----------------------------------------------------------------------------
template<typename Number>
TReal<Number>
operator + (const Number& lhs, const TReal<Number>& rhs)
{
    return rhs + lhs;
}
    
//-----------------------------------------------------------------------------
template<typename Number>
TReal<Number>
operator - (const Number& lhs, const TReal<Number>& rhs)
{
    return TReal<Number> (lhs) - rhs;
}
    
//-----------------------------------------------------------------------------
template<typename Number>
TReal<Number>
operator * (const Number& lhs, const TReal<Number>& rhs)
{
    return rhs * lhs;
}
    
//-----------------------------------------------------------------------------
template<typename Number>
TReal<Number>
operator / (const Number& lhs, const TReal<Number>& rhs)
{
    return TReal<Number> (lhs) / rhs;
}
    
//-----------------------------------------------------------------------------
template<typename Number>
TReal<Number>
operator % (const Number& lhs, const TReal<Number>& rhs)
{
    return TReal<Number> (lhs) % rhs;
}
    
//-----------------------------------------------------------------------------
// stream operator for real types
//-----------------------------------------------------------------------------
template<typename Number>
std::ostream& 
operator << (std::ostream& stream, const TReal<Number>& real)
{
    TReal<Number>   copy = real;
    TReal<Number>::CheckValid (copy, false, true);
    return stream << copy ();
}

//-----------------------------------------------------------------------------

#endif  //  _REAL_INL_
