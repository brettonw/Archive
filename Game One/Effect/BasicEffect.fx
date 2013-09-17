//-----------------------------------------------------------------------------
// Copyright (C) 1997-2006 Bretton Wade
// All Rights Reserved
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------

float3		g_lightDir;
float4		g_lightColor;
float4		g_color;
texture		g_diffuseTexture;

float4x4	g_projectionTransform;
float4x4	g_viewTransform;
float4x4	g_worldTransform;

//-----------------------------------------------------------------------------
// Texture samplers
//-----------------------------------------------------------------------------
sampler MeshTextureSampler = 
sampler_state
{
    Texture = <g_diffuseTexture>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

//-----------------------------------------------------------------------------
// Shared function for transforming vertices
//-----------------------------------------------------------------------------
float4 
TransformPos (float3 position)
{
    // compose the transforms, this should really be done once in the scene
    // graph node that sets the transforms for efficiency
    float4x4	transform = mul (g_worldTransform, g_viewTransform);
    transform = mul (transform, g_projectionTransform);
    
    // transform the position to screen space
    return mul (float4 (position, 1), transform);
}

//-----------------------------------------------------------------------------
// Vertex shader input structure
//-----------------------------------------------------------------------------
struct Vertex
{
	float3	position	: POSITION;
    float3	normal		: NORMAL;
    float2	texCoord	: TEXCOORD;
};

//-----------------------------------------------------------------------------
// Vertex shader output structure
//-----------------------------------------------------------------------------
struct Fragment
{
	float4	position	: POSITION;
	float3	normal		: COLOR;
    float2	texCoord	: TEXCOORD;
};

//-----------------------------------------------------------------------------
// Vertex Shader
//-----------------------------------------------------------------------------
Fragment 
UniversalVS (Vertex input)
{
	// set up the output structure
    Fragment	output;

    // transform the position to screen space
    output.position = TransformPos (input.position);
    
    // copy the normal through
	output.normal = normalize (mul (input.normal, (float3x3) g_worldTransform));
    
    // copy the texture coordinate through
	output.texCoord = input.texCoord; 

	// pass out the output
    return output;
}

//-----------------------------------------------------------------------------
// Pixel shader output structure
//-----------------------------------------------------------------------------
struct Pixel
{
    float4	color		: COLOR;
};

//-----------------------------------------------------------------------------
// Pixel Shader
//-----------------------------------------------------------------------------
Pixel 
AmbientPS (Fragment input) 
{ 
	// set up the output structure
    Pixel   output;
    
    // calculate ambient lighting according to exposure to the sky, straight up
    // is full bright, horizontal facing is 0.5, etc., and modulate the computed 
    // color by the texture
    output.color = tex2D (MeshTextureSampler, input.texCoord) * (dot (input.normal, -g_lightDir) + 1) * 0.5f * g_color * g_lightColor;

	// pass out the output
    return output;
}

//-----------------------------------------------------------------------------
Pixel 
DiffusePS (Fragment input) 
{ 
	// set up the output structure
    Pixel   output;
    
    // compute the basic lighting equation, subtract a bit to soften the edge 
    // of the shadow. The exact amount is calculated to be 2/32nd of a circle
    // to coincide with cylinder tessellations employed in the scene
    float	diffuse = dot (input.normal, -g_lightDir) - .196;
    diffuse = clamp (diffuse, 0, 1);
    output.color.rgb = tex2D (MeshTextureSampler, input.texCoord) * g_color * diffuse * g_lightColor;   
    output.color.a = 1.0f; 

	// pass out the output
    return output;
}

//-----------------------------------------------------------------------------
// Vertex shader input structure
//-----------------------------------------------------------------------------
struct ShadowVertex
{
	float3	position	: POSITION;
};

//-----------------------------------------------------------------------------
// Vertex shader output structure
//-----------------------------------------------------------------------------
struct ShadowFragment
{
	float4	position	: POSITION;
};

//-----------------------------------------------------------------------------
// Vertex Shader
//-----------------------------------------------------------------------------
ShadowFragment 
ShadowVS (ShadowVertex input)
{
	// set up the output structure
    ShadowFragment  output;

    // transform the position to screen space
    output.position = TransformPos (input.position);
    
	// pass out the output
    return output;
}

//-----------------------------------------------------------------------------
// Renders scene to render target
//-----------------------------------------------------------------------------
technique Render
{
    pass Ambient
    {          
        // set the cull mode
        CullMode = CCW;
        
        // debugging
        //FillMode = WIREFRAME;
        
        // set the VS and PS
        VertexShader = compile vs_2_0 UniversalVS ();
        PixelShader  = compile ps_2_0 AmbientPS ();
    }
    
    pass ShadowStencil
    {
        // set the cull mode to enable two sided stencil
        CullMode = None;
        
        // disable writing to the frame buffer
        ColorWriteEnable = 0;
        
        // disable writing to depth buffer
        ZWriteEnable = false;
        ZFunc = Less;
        
        // set up stencil states for z-pass rendering
        StencilEnable = True;
        TwoSidedStencilMode = True;
        StencilFunc = Always;
        CCW_StencilFunc = Always;
		StencilPass = Decr;
		CCW_StencilPass = Incr;
        
        // set the VS and PS
        VertexShader = compile vs_2_0 ShadowVS ();
        //PixelShader  = 0;
	}
    
    pass Lighting
    {
        // set the cull mode
        CullMode = CCW;
        
        // disable writing to the depth -buffer
		ZWriteEnable = False;

		// enable the frame buffer for adding the diffuse lighting contribution
        AlphaBlendEnable = True;
        BlendOp = Add;
        DestBlend = One;

        // set up stencil states for rendering if the stencil is 0
        StencilEnable = True;
        StencilRef = 0;
        StencilFunc = Equal;
        StencilPass = Keep;
        
        // set the VS and PS
        VertexShader = compile vs_2_0 UniversalVS ();
        PixelShader  = compile ps_2_0 DiffusePS ();
    }

}

//-----------------------------------------------------------------------------
