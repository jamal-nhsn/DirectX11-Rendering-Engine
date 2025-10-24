/*======
GLOBALS
======*/
cbuffer MatrixBuffer
{
    matrix viewMatrix;
    matrix projectionMatrix;
};

/*======
TYPEDEFS
======*/
struct VertexInputType
{
    float4 position : POSITION;
    float2 texcoord : TEXCOORD0;
    float4 color : COLOR;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD0;
    float4 color : COLOR;
};

/*===========
VERTEX SHADER
===========*/
PixelInputType VertexMain(VertexInputType input)
{
    PixelInputType output;
    
    // Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

    // Calculate the position of the vertex against the view and projection matrices.
    output.position = mul(input.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
    // Store the input variables for the pixel shader to use.
    output.texcoord = input.texcoord;
    output.color = input.color;
    
    return output;
}