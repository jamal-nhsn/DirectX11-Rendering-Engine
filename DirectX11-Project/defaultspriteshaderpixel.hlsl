/*=====
GLOBALS
=====*/
SamplerState sampleType[16] : register(s0);
Texture2D shaderTexture[128] : register(t0);

/*======
TYPEDEFS
======*/
struct PixelInputType
{
    float4 position : POSITION;
    float2 texcoord : TEXCOORD;
    float4 color : COLOR;
    float samplerIndex : SAMPLER_INDEX;
    float textureIndex : TEXTURE_INDEX;
};

/*==========
PIXEL SHADER
==========*/
float4 PixelMain(PixelInputType input) : SV_TARGET
{
    float4 textureColor;

    // Sample the pixel color from the proper texture using the proper sampler.
    textureColor = shaderTexture[(int)input.textureIndex].Sample(sampleType[(int)input.samplerIndex], input.texcoord);

    return textureColor * input.color;
}