//-----------------------------------------------------------------------------
// Copyright (C) 1997-2006 Bretton Wade
// All Rights Reserved
//-----------------------------------------------------------------------------

#ifndef     _DX_WRAPPER_H_
#define     _DX_WRAPPER_H_

//-----------------------------------------------------------------------------
// class definition for a simple com wrapper to automagically release objects
// when the wrapper goes out of scope
//-----------------------------------------------------------------------------
template<class aType>
class DXWrapper
{
    public:
        /* void */              DXWrapper (void) : 
                                    m_pDXObject (0) {}
                                    
        /* void */              DXWrapper (aType* pDXObject) : 
                                    m_pDXObject (pDXObject) {}
                                    
        /* void */              DXWrapper (const DXWrapper<aType>& COMObject) : 
                                    m_pDXObject (COMObject.m_pDXObject)
                                    {if (m_pDXObject) m_pDXObject->AddRef ();}
                                    
        /* void */              ~DXWrapper (void)
                                    { if (m_pDXObject) m_pDXObject->Release (); }
                                    
        DXWrapper<aType>&       operator = (const DXWrapper<aType>& pDXObject)
                                    {
                                        if (pDXObject.m_pDXObject)
                                            pDXObject->AddRef (); 
                                        if (m_pDXObject)
                                            m_pDXObject->Release ();
                                        m_pDXObject = pDXObject.m_pDXObject; 
                                        return *this;
                                    }
        
        aType*                  operator -> (void) const
                                    { return m_pDXObject; }
                                    
        /* aType* */            operator aType* (void) const
                                    { return m_pDXObject; }
                                    
        aType**                 operator & (void)
                                    { return & m_pDXObject; }
        
    private:
        aType*                  m_pDXObject;
};

//-----------------------------------------------------------------------------

#endif  //  _DX_WRAPPER_H_
