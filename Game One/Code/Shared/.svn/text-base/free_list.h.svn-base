//-----------------------------------------------------------------------------
// Copyright (C) 1997-2006 Bretton Wade
//-----------------------------------------------------------------------------

#ifndef     _FREE_LIST_H_
#define     _FREE_LIST_H_

//-----------------------------------------------------------------------------
// types
//-----------------------------------------------------------------------------
typedef void*   FreePtr;

//-----------------------------------------------------------------------------
// class definitions
//-----------------------------------------------------------------------------
template<class aType, int iBlockSize = 32>
class FreeList
{
    public:
        FreePtr     operator new (size_t size DEBUG_NEW_PARAMETERS)
        {
            Assert (size == sizeof (aType));
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
                        operator delete (pFreeBlock);
                        pFreeBlock += sizeof (aType);
                    }
                }
            }
            
            // under normal circumstances, there should be empty blocks on the free 
            // list when we get here, so we'll take the top of the stack. Just in case
            // there isn't anything on the free list, we'll check before we do the
            // pop operation to reset the free list value
            FreePtr    pFreeBlock = s_pFreeList;
            if (s_pFreeList)
                s_pFreeList = *(reinterpret_cast<FreePtr*> (pFreeBlock));
                
            // return the block (if any) that we popped off the top of the stack
            return pFreeBlock;
        }
        
        FreePtr     operator new[] (size_t size DEBUG_NEW_PARAMETERS)
        {
            // when allocating arrays, we'll default to the global new operator
            return ::operator NewCall[] (size);
        }
        
        void        operator delete (FreePtr pFreeBlock)
        {
            // this is done like a stack, the current free list pointer is 
            // stored in the untyped memory, and the pointer to that location
            // is set as the new free list pointer
            *(reinterpret_cast<FreePtr*> (pFreeBlock)) = s_pFreeList;
            s_pFreeList = pFreeBlock;
        }
        
        void        operator delete[] (FreePtr ptrArray)
        {
            // similar the the operator new[], we'll default to the global delete
            // operator when working with arrays
            ::operator delete[] (ptrArray);
        }

#ifndef     NDEBUG
        // the compiler will complain that there is an unmatched delete
        // operator when using debug new (C4291 in VC7), and exception handling
        // is enabled. This is because the compiler automatically generates 
        // code to call delete if the constructor for a class throws an 
        // exception. To shut the compiler up, we provide matching params
        void        operator delete (FreePtr pFreeBlock DEBUG_NEW_PARAMETERS)
        {
            operator delete (pFreeBlock);
        }
        
        void        operator delete[] (FreePtr ptrArray DEBUG_NEW_PARAMETERS)
        {
            operator delete[] (ptrArray);
        }
#endif

    private:
static  FreePtr     s_pFreeList;
};

//-----------------------------------------------------------------------------
// template static members
//-----------------------------------------------------------------------------
template<class aType, int iBlockSize>
FreePtr     FreeList<aType, iBlockSize>::s_pFreeList = 0;

//-----------------------------------------------------------------------------
// macros
//-----------------------------------------------------------------------------

// this macro is useful when using free lists on derived classes, to tell the 
// compiler what inherited new/delete operators to use
#define     USING_FREE_LIST(aType)                                              \
    using FreeList<aType>::operator new;                                        \
    using FreeList<aType>::operator new[];                                      \
    using FreeList<aType>::operator delete;                                     \
    using FreeList<aType>::operator delete[]

//-----------------------------------------------------------------------------

#endif  //  _FREE_LIST_H_
