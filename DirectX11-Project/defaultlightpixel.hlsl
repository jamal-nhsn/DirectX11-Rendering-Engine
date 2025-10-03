/*======
INCLUDES
======*/
#include "light.h"

/*=====
GLOBALS
=====*/
Texture2D shaderTexture : register(t0);
SamplerState sampleType : register(s0);

cbuffer LightBuffer
{
    Light light;
};

cbuffer MaterialBuffer
{
    float4 specularTint;
    float  shininess;
    float3 padding;
};

/*======
TYPEDEFS
======*/
struct PixelInputType
{
    float4 position : SV_POSITION;
    float3 normal   : NORMAL;
    float2 tex      : TEXCOORD;
    float3 viewDir  : VIEWDIR;
};

/*==========
PIXEL SHADER
==========*/
float4 PixelMain(PixelInputType input) : SV_TARGET
{
    float4 textureColor;
    float3 lightDirection;
    float3 normal;
    float lightIntensity;
    float3 reflection;
    
    float4 diffuse  = 0.0f;
    float4 specular = 0.0f;
    
    // Sample the pixel color from the texture using the sampler at this texture coordinate location.
    textureColor = shaderTexture.Sample(sampleType, input.tex);

    // Invert the light direction for calculations.
    lightDirection = -light.direction;
    
    // Calculate the amount of light on this pixel.
    normal = normalize(input.normal);
    lightIntensity = dot(normal, lightDirection);
    
    // Only find the diffuse and specular contribution if the intensity is positive.
    if (lightIntensity > 0.0f)
    {
        // Determine the final amount of diffuse color based on the diffuse color combined with the light intensity.
        diffuse = saturate(light.color * saturate(lightIntensity));
    
        // Calculate the reflection vector based on the light intensity, normal vector, and light direction.
        reflection = normalize(2.0f * lightIntensity * normal - lightDirection);
    
        // Determine the amount of specular light based on the reflection vector, viewing direction, and shininess.
        specular = light.color * specularTint * pow(saturate(dot(reflection, input.viewDir)), shininess);

        // Multiply the texture pixel and the final diffuse color to get the final pixel color result.
        diffuse = diffuse * textureColor;
    }
    
    // Return the sum of the diffuse and specular components.
    return saturate(diffuse + specular);
}