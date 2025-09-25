/*======
GLOBALS
======*/
cbuffer MatrixBuffer
{
    matrix modelMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

/*======
TYPEDEFS
======*/
struct VertexInputType
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 tex : TEXCOORD0;
};

struct PixelInputType
{
    float3 position : SV_POSITION;
    float3 normal: NORMAL;
    float2 tex : TEXCOORD0;
};

/*===========
VERTEX SHADER
===========*/
PixelInputType VertexMain(VertexInputType input)
{
    PixelInputType output;
    
    // Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

    // Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, modelMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

    // Calculate the normals in world space.
    output.normal = mul(input.position, (float3x3)modelMatrix);

    // Normalize the normals.
    output.normal = normalize(output.normal);
    
    // Store the input texcoord for the pixel shader to use.
    output.tex = input.tex;
    
    return output;
}