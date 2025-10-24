/*=====
GLOBALS
=====*/
Texture2D shaderTexture : register(t0);
SamplerState sampleType : register(s0);

/*======
TYPEDEFS
======*/
struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD0;
    float4 color : COLOR;
};

/*==========
PIXEL SHADER
==========*/
float4 PixelMain(PixelInputType input) : SV_TARGET
{
    // Sample the pixel color from the proper texture.
    float4 textureColor = shaderTexture.Sample(sampleType, input.texcoord);

    // Tint the output by the color.
    return textureColor * input.color;

}