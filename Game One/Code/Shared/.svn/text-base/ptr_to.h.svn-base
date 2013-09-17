//-----------------------------------------------------------------------------
// Copyright (C) 1997-2006 Bretton Wade
// All rights reserved.
//-----------------------------------------------------------------------------

#ifndef     _PTR_TO_H_
#define     _PTR_TO_H_

//-----------------------------------------------------------------------------
// include files
//-----------------------------------------------------------------------------
#ifndef     _COUNTED_OBJECT_H_
#include    "counted_object.h"
#endif  //  _COUNTED_OBJECT_H_

//-----------------------------------------------------------------------------
// class definitions
//-----------------------------------------------------------------------------
template <class aType>
class   PtrTo
{
    public:
        /* void */              PtrTo (void);
        /* void */              PtrTo (aType* ptr);
        /* void */              PtrTo (const PtrTo<aType>& ptr);
virtual /* void */              ~PtrTo (void);

        const aType*            GetPtr (void) const;
        const aType&            operator * (void) const;
        const aType*            operator -> (void) const;
        
        aType*                  GetPtr (void);
        aType&                  operator * (void);
        aType*                  operator -> (void);
        
        /* void* */             operator const void* (void) const;
        
        PtrTo<aType>&           operator = (aType* ptr);
        PtrTo<aType>&           operator = (const PtrTo<aType>& ptr);
        
        bool                    operator == (const PtrTo<aType>& ptr);
        bool                    operator != (const PtrTo<aType>& ptr);

        bool                    IsUnique (void) const;
        void                    MakeUnique (void);

    protected:
        aType*                  m_ptr;
};

//-----------------------------------------------------------------------------
// macros
//-----------------------------------------------------------------------------
#define MAKE_PTR_TO(aType)                                                      \
    class aType;                                                                \
    typedef PtrTo<aType>       PtrTo ## aType

//-----------------------------------------------------------------------------
// inlines
//-----------------------------------------------------------------------------
#ifndef     _PTR_TO_INL_
#include    "ptr_to.inl"
#endif  //  _PTR_TO_INL_

//-----------------------------------------------------------------------------

#endif  //  _PTR_TO_H_
