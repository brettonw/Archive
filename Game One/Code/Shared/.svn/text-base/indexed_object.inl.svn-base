//-----------------------------------------------------------------------------
// Copyright (C) 1997-2006 Bretton Wade
// All Rights Reserved
//-----------------------------------------------------------------------------

#ifndef     _INDEXED_OBJECT_INL_
#define     _INDEXED_OBJECT_INL_

//-----------------------------------------------------------------------------
// include files
//-----------------------------------------------------------------------------
#ifndef     _INDEXED_OBJECT_H_
#include    "indexed_object.h"
#endif  //  _INDEXED_OBJECT_H_

//-----------------------------------------------------------------------------
// template static members
//-----------------------------------------------------------------------------
template<class someType>
TextMap<PtrTo<someType> >                   IndexedObject<someType>::s_indexByName;
template<class someType>
Map<uInt2, PtrTo<someType> >                IndexedObject<someType>::s_indexByID;
template<class someType>
uInt2                                       IndexedObject<someType>::s_ID;
template<class someType>
TextMap<typename IndexedObject<someType>::Factory*>* IndexedObject<someType>::s_pFactoryMap = 0;

//-----------------------------------------------------------------------------
// template methods
//-----------------------------------------------------------------------------
template<class someType>
const Text&
IndexedObject<someType>::GetName (void) const
{
    return m_name;
}

//-----------------------------------------------------------------------------
template<class someType>
uInt2
IndexedObject<someType>::GetID (void) const
{
    return m_ID;
}

//-----------------------------------------------------------------------------
template<class someType>
PtrTo<someType>
IndexedObject<someType>::Create (const PtrToXMLNode& pXMLNode)
{
    // create the object at its factory, check for success
    PtrTo<someType> pSomeType = Factory::Create (pXMLNode);
    if (pSomeType)
    {
        // register the object by name
        Text    name = pSomeType->GetName ();
        Assert ((s_indexByName.find (name) == s_indexByName.end ()) and "names must be unique");
        s_indexByName[name] = pSomeType;

        // register the object by id
        uInt2   ID = pSomeType->GetID ();
        Assert ((s_indexByID.find (ID) == s_indexByID.end ()) and "IDs must be unique");
        s_indexByID[ID] = pSomeType;
    }
    
    // return the created object
    return pSomeType;
}

//-----------------------------------------------------------------------------
template<class someType>
PtrTo<someType>
IndexedObject<someType>::Find (const Text& name)
{
    // check to see if the name is in the index
    TextMap<PtrTo<someType> >::iterator iter = s_indexByName.find (name);
    if (iter != s_indexByName.end ())
    {
        // return the found object
        return iter->second;
    }
        
    // return null to indicate failure
    return 0;
}

//-----------------------------------------------------------------------------
template<class someType>
PtrTo<someType>
IndexedObject<someType>::Find (uInt2 ID)
{
    // check to see if the id is in the index
    Map<uInt2, PtrTo<someType> >::iterator  iter = s_indexByID.find (ID);
    if (iter != s_indexByID.end ())
    {
        // return the found object
        return iter->second;
    }
        
    // return null to indicate failure
    return 0;
}

//-----------------------------------------------------------------------------
template<class someType>
void
IndexedObject<someType>::Remove (const Text& name)
{
    // check to see if the name is in the index
    TextMap<PtrTo<someType> >::iterator iter = s_indexByName.find (name);
    if (iter != s_indexByName.end ())
    {
        // fetch the object
        PtrTo<someType>     pObject = iter->second;
        
        // remove it from the index, this will automatically decrement
        // the reference counted pointer
        s_indexByName.erase (iter);
        
        // remove it from the index by ID
        Remove (pObject->GetID ());
    }
}

//-----------------------------------------------------------------------------
template<class someType>
void
IndexedObject<someType>::Remove (uInt2 ID)
{
    // check to see if the id is in the index
    Map<uInt2, PtrTo<someType> >::iterator  iter = s_indexByID.find (ID);
    if (iter != s_indexByID.end ())
    {
        // fetch the object
        PtrTo<someType>     pObject = iter->second;
        
        // remove it from the index, this will automatically decrement
        // the reference counted pointer
        s_indexByName.erase (iter);
        
        // remove it from the index by name
        Remove (pObject->GetName ());
    }
}

//-----------------------------------------------------------------------------
template<class someType>
void
IndexedObject<someType>::Remove (const PtrTo<someType>& pObject)
{
    // do it by name and id
    Remove (pObject->GetName ());
    Remove (pObject->GetID ());
}

//-----------------------------------------------------------------------------
template<class someType>
void
IndexedObject<someType>::RemoveAll (void)
{
    // clear out the indices
    s_indexByName.clear ();
    s_indexByID.clear ();
}

//-----------------------------------------------------------------------------
template<class someType>
/* void */
IndexedObject<someType>::IndexedObject (const PtrToXMLNode& pXMLNode)
{
    // get the name from the xml
    m_name = pXMLNode->GetAttribute ("name");
    
    // generate a unique id from the static id
    m_ID = ++s_ID;

    DebugPrint 
    (
        "Create Object \"%s\", id = %05hu, type = \"%s\"\n", 
		(cString) m_name, 
		m_ID, 
		(cString) ((pXMLNode->GetAttribute ("type").Length () > 0) ? pXMLNode->GetAttribute ("type") : pXMLNode->GetName ())
	);
}

//-----------------------------------------------------------------------------

#endif  //  _INDEXED_OBJECT_INL_
