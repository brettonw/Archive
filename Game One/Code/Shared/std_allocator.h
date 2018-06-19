//-----------------------------------------------------------------------------
// Copyright (C) 1997-2006 Bretton Wade
//-----------------------------------------------------------------------------

#ifndef     _STD_ALLOCATOR_H_
#define     _STD_ALLOCATOR_H_

//-----------------------------------------------------------------------------
// compiler check - the following code is only used if you have VC.net
//-----------------------------------------------------------------------------
//#if         _MSC_VER >= 1300
#if 0

//-----------------------------------------------------------------------------
// include files
//-----------------------------------------------------------------------------
#ifndef     _XMEMORY_
#include    <xmemory>
#endif  //  _XMEMORY_

//-----------------------------------------------------------------------------
// types
//-----------------------------------------------------------------------------
typedef void*   AllocFreePtr;

//-----------------------------------------------------------------------------
// class definitions
//-----------------------------------------------------------------------------
template<class aType, int iBlockSize>
class StdAllocator
{
    public:
	    typedef aType           value_type;
	    typedef size_t          size_type;
	    typedef ptrdiff_t       difference_type;
	    typedef aType*          pointer;
	    typedef const aType*    const_pointer;
	    typedef aType&          reference;
	    typedef const aType&    const_reference;
	    
    public:
        /* void */              StdAllocator (void) {}
	    aType*                  allocate (size_t count, const void* pHint);
	    aType*                  allocate (size_t count);
	    void                    deallocate (aType* ptr, size_t count);
	    aType*                  address (aType& val) const;
	    const aType*            address (const aType& val) const;
	    void                    construct (aType* ptr, const aType& val);
	    void                    destroy (aType* ptr);
	    size_t                  max_size (void) const;

    protected:
        AllocFreePtr            MyAllocate (void);
        void                    MyDeallocate (AllocFreePtr pFreeBlock);
        
    protected:
static  AllocFreePtr            s_pFreeList;

    public:
	    template<class otherType>
		struct                              rebind { typedef StdAllocator<otherType, iBlockSize> other; };

        template<class otherType, int iOtherBlockSize>
        /* void */                          StdAllocator (const StdAllocator<otherType, iOtherBlockSize>&) {}

        template<class otherType, int iOtherBlockSize>
        StdAllocator<aType, iBlockSize>&    operator = (const StdAllocator<otherType, iOtherBlockSize>&) { return (*this); }
};

//-----------------------------------------------------------------------------
// template static members
//-----------------------------------------------------------------------------
template<class aType, int iBlockSize>
AllocFreePtr    StdAllocator<aType, iBlockSize>::s_pFreeList = 0;

//-----------------------------------------------------------------------------
// template members
//-----------------------------------------------------------------------------
template<class aType, int iBlockSize>
aType*                  
StdAllocator<aType, iBlockSize>::allocate (size_t count, const void* pHint)
{ 
	if (count == 1)
		return reinterpret_cast<aType*> (MyAllocate ());
	else
		return reinterpret_cast<aType*> (NewCall char (count * sizeof (aType)));
}

//-----------------------------------------------------------------------------
template<class aType, int iBlockSize>
aType*                  
StdAllocator<aType, iBlockSize>::allocate (size_t count)
{ 
    return allocate (count, 0); 
}

//-----------------------------------------------------------------------------
template<class aType, int iBlockSize>
void                    
StdAllocator<aType, iBlockSize>::deallocate (aType* ptr, size_t count)
{ 
	if (count == 1) 
		MyDeallocate (ptr); 
	else 
		operator delete (ptr);
}

//-----------------------------------------------------------------------------
template<class aType, int iBlockSize>
aType*                  
StdAllocator<aType, iBlockSize>::address (aType& val) const
{ 
    return (&val); 
}

//-----------------------------------------------------------------------------
template<class aType, int iBlockSize>
const aType*            
StdAllocator<aType, iBlockSize>::address (const aType& val) const
{ 
    return (&val); 
}

//-----------------------------------------------------------------------------
template<class aType, int iBlockSize>
void                    
StdAllocator<aType, iBlockSize>::construct (aType* ptr, const aType& val)
{ 
    std::_Construct (ptr, val); 
}

//-----------------------------------------------------------------------------
template<class aType, int iBlockSize>
void                    
StdAllocator<aType, iBlockSize>::destroy (aType* ptr)
{ 
    std::_Destroy (ptr); 
}

//-----------------------------------------------------------------------------
template<class aType, int iBlockSize>
size_t                   
StdAllocator<aType, iBlockSize>::max_size (void) const
{ 
    size_t count = (size_t)(-1) / sizeof (aType); 
    return (0 < count) ? count : 1; 
}

//-----------------------------------------------------------------------------
template<class aType, int iBlockSize>
AllocFreePtr
StdAllocator<aType, iBlockSize>::MyAllocate (void)
{
    // check to see if there are any free objects
    if (not s_pFreeList)
    {
        // there weren't any free objects ready for taking, so we need to
        // allocate a chunk of them and check for success
        // NOTE - this memory will never be recovered since we effectively
        // forget what the original pointer is. This shouldn't be a problem
        // in practice, since the objects allocated and stored in the free
        // list will be constantly recycled on a global basis.
        NEW_BLOCK_TYPE (_CRT_BLOCK);
        char*   pFreeBlock = NewCall char[sizeof (aType) * iBlockSize];
        if (pFreeBlock)
        {   
            // we got a chunk of memory, so now we want to break it up and
            // populate the free list with the pieces
            for (uInt i = 0; i < iBlockSize; i++)
            {
                // We treat this as if we are deallocating a block of memory,
                // then the walking pointer gets incremented to the next 
                // location in memory that will satisfy the size requirements 
                // for the object type we are allocating.
                MyDeallocate (pFreeBlock);
                pFreeBlock += sizeof (aType);
            }
        }
    }
    
    // under normal circumstances, there should be empty blocks on the free 
    // list when we get here, so we'll take the top of the stack. Just in case
    // there isn't anything on the free list, we'll check before we do the
    // pop operation to reset the free list value
    AllocFreePtr    pFreeBlock = s_pFreeList;
    if (s_pFreeList)
        s_pFreeList = *(reinterpret_cast<AllocFreePtr*> (pFreeBlock));
        
    // return the block (if any) that we popped off the top of the stack
    return pFreeBlock;
}

//-----------------------------------------------------------------------------
template<class aType, int iBlockSize>
void        
StdAllocator<aType, iBlockSize>::MyDeallocate (AllocFreePtr pFreeBlock)
{
    // this is done like a stack, the current free list pointer is 
    // stored in the untyped memory, and the pointer to that location
    // is set as the new free list pointer
    *(reinterpret_cast<AllocFreePtr*> (pFreeBlock)) = s_pFreeList;
    s_pFreeList = pFreeBlock;
}

//-----------------------------------------------------------------------------
// standard container types using this allocator
//-----------------------------------------------------------------------------
template<class keyType, class valueType, int iBlockSize = 64>
class Map : public std::map<keyType, valueType, std::less<keyType>, StdAllocator<std::pair<const keyType, valueType>, iBlockSize> > {};

template<class aType, int iBlockSize = 64>
class List : public std::list<aType, StdAllocator<aType, iBlockSize> > {};

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// compiler check - the following code is only used if you have VC6
//-----------------------------------------------------------------------------
#else   //  _MSC_VER >= 1300

//-----------------------------------------------------------------------------
// standard container types not using any allocator
//-----------------------------------------------------------------------------
template<class keyType, class valueType, int iBlockSize = 64>
class Map : public std::map<keyType, valueType> {};

template<class aType, int iBlockSize = 64>
class List : public std::list<aType> {};

//-----------------------------------------------------------------------------
#endif  //  _MSC_VER >= 1300

#endif  //  _STD_ALLOCATOR_H_
