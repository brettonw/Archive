//-----------------------------------------------------------------------------
// Copyright (C) 1997-2002 Bretton Wade
// All rights reserved.
//-----------------------------------------------------------------------------

#ifndef     _SINGLETON_H_
#define     _SINGLETON_H_

//-----------------------------------------------------------------------------
// class definitions
//-----------------------------------------------------------------------------
template <class aType>
class   Singleton
{
    public:
        /* void */              Singleton (void);
virtual /* void */              ~Singleton (void);

static  bool                    IsValid (void);
static  aType*                  GetSingleton (void);
    
    private:
static  aType*                  s_pSingleton;
};

//-----------------------------------------------------------------------------
// inlines
//-----------------------------------------------------------------------------
#ifndef     _SINGLETON_INL_
#include    "singleton.inl"
#endif  //  _SINGLETON_INL_

//-----------------------------------------------------------------------------

#endif  //  _SINGLETON_H_
