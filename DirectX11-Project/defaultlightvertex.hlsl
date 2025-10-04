/*======
GLOBALS
======*/
cbuffer MatrixBuffer
{
    matrix modelMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

cbuffer CameraBuffer
{
    float3 cameraPosition;
    float  padding;
};

/*======
TYPEDEFS
======*/
struct VertexInputType
{
    float4 position : POSITION;
    float3 normal : NORMAL;
    float2 tex : TEXCOORD0;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float4 worldPos : WORLD_POS;
    float3 normal   : NORMAL;
    float2 tex      : TEXCOORD;
    float3 viewDir  : VIEWDIR;
};

/*===========
VERTEX SHADER
===========*/
PixelInputType VertexMain(VertexInputType input)
{
    PixelInputType output;
    
    // Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

    // Calculate the position of the vertex in world space.
    output.worldPos = mul(input.position, modelMatrix);
    
    // Calculate the view direction.
    output.viewDir = normalize(cameraPosition - output.worldPos.xyz);
    
    // Multiply the world space position by the view, and projection matrices to get the position in clip space.
    output.position = mul(output.worldPos, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

    // Calculate the normals in world space.
    output.normal = mul(input.normal, (float3x3)modelMatrix);

    // Normalize the normals.
    output.normal = normalize(output.normal);
    
    // Store the input texcoord for the pixel shader to use.
    output.tex = input.tex;
    
    return output;
}