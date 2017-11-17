//-----------------------------------------------------------------------------
// Copyright (C) 1997-2006 Bretton Wade
// All rights reserved.
//-----------------------------------------------------------------------------

#ifndef     _PTR_TO_H_
#define     _PTR_TO_H_

#ifndef     _COUNTED_OBJECT_H_
#include    "counted_object.h"
#endif  //  _COUNTED_OBJECT_H_

template <class aType>
class   PtrTo {
    public:
    PtrTo (void);
    PtrTo (aType* ptr);
    PtrTo (const PtrTo<aType>& ptr);
    virtual ~PtrTo (void);

    aType* setPtr (aType* ptr);
    const aType* getPtr (void) const;
    const aType& operator * (void) const;
    const aType* operator -> (void) const;

    aType* getPtr (void);
    aType& operator * (void);
    aType* operator -> (void);

    operator const void* (void) const;

    PtrTo<aType>& operator = (aType* ptr);
    PtrTo<aType>& operator = (const PtrTo<aType>& ptr);

    bool operator == (const PtrTo<aType>& ptr);
    bool operator != (const PtrTo<aType>& ptr);

    bool isUnique (void) const;
    void makeUnique (void);

    protected:
    aType* ptrTo;
};

#define MAKE_PTR_TO(aType)                                                      \
    class aType;                                                                \
    typedef PtrTo<aType>       PtrTo ## aType

#ifndef     _PTR_TO_INL_
#include    "ptr_to.inl"
#endif  //  _PTR_TO_INL_

#endif  //  _PTR_TO_H_
