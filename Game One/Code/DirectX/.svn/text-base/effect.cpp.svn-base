//-----------------------------------------------------------------------------
// Copyright (C) 1997-2006 Bretton Wade
// All Rights Reserved
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// include files
//-----------------------------------------------------------------------------
#include    "precompile.h"
#include    "effect.h"
#include    "direct_3d.h"

//-----------------------------------------------------------------------------
// static class members
//-----------------------------------------------------------------------------
DEFINE_FACTORY (Effect);
PtrToEffect Effect::s_pCurrent;

//-----------------------------------------------------------------------------
// class methods
//-----------------------------------------------------------------------------
void                    
Effect::Select (const PtrToEffect& pEffect)
{
    s_pCurrent = pEffect;
}

//-----------------------------------------------------------------------------
void                    
Effect::Clear (void)
{
    s_pCurrent = 0;
}

//-----------------------------------------------------------------------------
PtrToEffect&            
Effect::GetCurrent (void)
{
    return s_pCurrent;
}

//-----------------------------------------------------------------------------
/* void */
Effect::Effect (const PtrToXMLNode& pXMLNode) :
    IndexedObject <Effect> (pXMLNode)
{
    // set the shader flags
    DWORD               dwShaderFlags = 0;
    #ifndef SHADER_FLAG_USE_PRESHADERS
    dwShaderFlags |= D3DXSHADER_NO_PRESHADER;
    #endif

    // fetch the direct 3d object
    Direct3D&           pDirect3D = *(Direct3D::GetSingleton ());
     
    // extract the filename
	Text                filename = "Effect\\" + pXMLNode->GetAttribute ("filename");

    // create the effect
    LPD3DXBUFFER        pErrorBuffer;
    if (FAILED (D3DXCreateEffectFromFile (*pDirect3D, filename.GetText (), 0, 0, dwShaderFlags, 0, &m_pEffect, &pErrorBuffer)))
    {
        // print the error to the output window so it can be double clicked in Visual Studio
        LPVOID pCompileErrors = pErrorBuffer->GetBufferPointer ();
        DebugPrint ((const char*) pCompileErrors);
        
        // bail out completely
        __asm { int 3 };
    }
    
    // select and validate the technique
    char*               techniqueName = "Render";
    D3DXHANDLE          technique = m_pEffect->GetTechniqueByName(techniqueName);
    if (D3D_OK == m_pEffect->ValidateTechnique (technique))
    {
        D3D_TEST (m_pEffect->SetTechnique (techniqueName));
    }
    else
    {
        // spew a message so that the file can be opened in visual studio
        DebugPrint ("%s(1): FAILED TO VALIDATE TECHNIQUE (%s)\n", filename.GetText (), techniqueName);
        
        // bail out completely
        __asm { int 3 };
    }
}

//-----------------------------------------------------------------------------
/* void */
Effect::~Effect (void)
{
}

//-----------------------------------------------------------------------------
