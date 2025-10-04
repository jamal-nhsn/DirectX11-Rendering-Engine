/*=====
GLOBALS
=====*/
Texture2D shaderTexture : register(t0);
SamplerState sampleType : register(s0);

cbuffer AmbientLight
{
    float4 ambientLight;
};

/*======
TYPEDEFS
======*/
struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
};

/*==========
PIXEL SHADER
==========*/
float4 PixelMain(PixelInputType input) : SV_TARGET
{
    float4 textureColor;

    // Sample the pixel color from the texture using the sampler at this texture coordinate location.
    textureColor = shaderTexture.Sample(sampleType, input.tex);

    float4 ambientColor = float4(ambientLight.xyz * ambientLight.w, 1.0f);
    
    // Multiply by the ambient light.
    return textureColor * ambientColor;
}