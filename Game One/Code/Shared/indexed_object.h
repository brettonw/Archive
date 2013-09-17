//-----------------------------------------------------------------------------
// Copyright (C) 1997-2006 Bretton Wade
// All Rights Reserved
//-----------------------------------------------------------------------------

#ifndef     _INDEXED_OBJECT_H_
#define     _INDEXED_OBJECT_H_

//-----------------------------------------------------------------------------
// include files
//-----------------------------------------------------------------------------
#ifndef     _XML_NODE_H_
#include    "xml_node.h"
#endif  //  _XML_NODE_H_

//-----------------------------------------------------------------------------
// class definitions
//-----------------------------------------------------------------------------
template<class someType>
class IndexedObject : public CountedObject
{
    // inherited interface for individual objects
    public:
        const Text&                 GetName (void) const;
        uInt2                       GetID (void) const;
        
    protected:
        Text                        m_name;
        uInt2                       m_ID;
        
    // inherited interface for classwide indexing
    public:
static  PtrTo<someType>             Create (const PtrToXMLNode& pXMLNode);
static  PtrTo<someType>             Find (const Text& name);
static  PtrTo<someType>             Find (uInt2 ID);
static  void                        Remove (const Text& name);
static  void                        Remove (uInt2 ID);
static  void                        Remove (const PtrTo<someType>& pObject);
static  void                        RemoveAll (void);

    protected:
static  TextMap<PtrTo<someType> >   s_indexByName;
static  Map<uInt2, PtrTo<someType> >s_indexByID;
static  uInt2                       s_ID;


    // what follows here is a support class for automatic registration of a 
    // factory object for each subclass of indexed object. The general idea 
    // is that XML nodes will be used for building the objects, and we want 
    // to add them to the index as they are created, so we must control the 
    // construction and destruction.
    protected:
        /* void */                  IndexedObject (const PtrToXMLNode& pXMLNode);
virtual /* void */                  ~IndexedObject (void) {}

    protected:
        struct Factory
        {
                Text                m_type;
                
                // this constructor allocates the factory map if it isn't
                // already in existence, and then installs the type factory
                // object in the map. Factory objects should only be created 
                // during static initialization using the support macros 
                // provided below.
                /* void */          Factory (const Text& type) :
                    m_type (type)
                {
                    if (not s_pFactoryMap)
                        s_pFactoryMap = NewCall TextMap<Factory*>;
                    (*s_pFactoryMap)[m_type] = this; 
                }
                    
                // this destructor removes the factory object from the map,
                // and deletes the map if it is empty. As with the constructor,
                // factory objects should only be getting destroyed during
                // static shutdown.
        virtual /* void */          ~Factory (void)
                {
                    Assert (s_pFactoryMap);
                    TextMap<Factory*>::iterator iter = s_pFactoryMap->find (m_type);
                    Assert (iter != s_pFactoryMap->end ());
                    s_pFactoryMap->erase (iter);
                    if (s_pFactoryMap->size () == 0)
                    {
                        delete s_pFactoryMap;
                        s_pFactoryMap = 0;
                    }
                    
                }

                // these methods do the actual work of creating a PtrTo<aType>
                // from an xml node
        virtual PtrTo<someType>     Build (const PtrToXMLNode& pXMLNode) = 0;
        static  PtrTo<someType>     Create (const PtrToXMLNode& pXMLNode)
                {
                    PtrTo<someType> pSomeType;
                    Text            type = pXMLNode->GetAttribute ("type");
					if (type.Length () == 0)
						type = pXMLNode->GetName ();
                    TextMap<Factory*>::iterator iter = s_pFactoryMap->find (type);
                    if (iter != s_pFactoryMap->end ())
                    {
                        Factory*    pFactory = iter->second;
                        pSomeType = pFactory->Build (pXMLNode);
                    }
                    return pSomeType;
                }
        };
        
static  TextMap<Factory*>*  s_pFactoryMap;
};

//-----------------------------------------------------------------------------
// macros
//-----------------------------------------------------------------------------
        // use this macro at the bottom of class definitions for classes that 
        // should be creatable from an XML node - it declares the constructor
        // and destructor for your class, so be sure those are defined 
        // elsewhere.
#define DECLARE_FACTORY(BaseObjectName, ObjectName)                             \
        public:                                                                 \
        ObjectName (const PtrToXMLNode& pXMLNode);                              \
        ~ObjectName (void);                                                     \
        struct Factory : public IndexedObject<BaseObjectName>::Factory          \
        {                                                                       \
                /* void */              Factory (void) :                        \
                    IndexedObject<BaseObjectName>::Factory (#ObjectName) {}     \
        virtual /* void */              ~Factory (void) {}                      \
        virtual PtrTo<BaseObjectName>   Build (const PtrToXMLNode& pXMLNode)    \
                    { return NewCall ObjectName (pXMLNode); }                   \
        };                                                                      \
        static  Factory s_Factory ## ObjectName

        // use this macro at the top of the source file that contains 
        // definitions of your class methods - it completes the static 
        // declaration of a factory object in class scope.
#define DEFINE_FACTORY(ObjectName)                                              \
        ObjectName::Factory ObjectName::s_Factory ## ObjectName

        // use this macro to load an object from an xml node (pXMLNode)
#define LOAD_FROM_XML(Object, pXMLNode)                                         \
    {                                                                           \
        const XMLNodeList*  pObjectList = pXMLNode->GetChildren (#Object);      \
        if (pObjectList)                                                        \
        {                                                                       \
            XMLNodeList::const_iterator iter = pObjectList->begin ();           \
            XMLNodeList::const_iterator end = pObjectList->end ();              \
            while (iter != end)                                                 \
            {                                                                   \
                PtrToXMLNode    pObjectNode = *iter++;                          \
                Object::Create (pObjectNode);                                   \
            }                                                                   \
        }                                                                       \
    }
//-----------------------------------------------------------------------------
// inlines
//-----------------------------------------------------------------------------
#ifndef     _INDEXED_OBJECT_INL_
#include    "indexed_object.inl"
#endif  //  _INDEXED_OBJECT_INL_

//-----------------------------------------------------------------------------

#endif  //  _INDEXED_OBJECT_H_
