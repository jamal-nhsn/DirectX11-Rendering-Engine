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
    LightData light;
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
    float4 worldPos : WORLD_POS;
    float3 normal   : NORMAL;
    float2 tex      : TEXCOORD;
    float3 viewDir  : VIEWDIR;
};

float4 DirectionalLightDiffuseSpecular(float4 textureColor, float4 lightColor, float3 normal, float3 viewDir)
{
    float4 diffuse = 0.0f;
    float4 specular = 0.0f;
    
    // Calculate the diffuse lighting contribution for the fragment.
    float lightDir = -light.direction;
    float diffuseFactor = saturate(dot(normal, lightDir));
    diffuse = textureColor * saturate(lightColor * diffuseFactor);
    
    // Calculate the specular lighting contribution for the fragment.
    float3 reflectDir = reflect(-lightDir, normal);
    float specularFactor = saturate(dot(reflectDir, viewDir));
    specular = lightColor * specularTint * specularFactor * pow(specularFactor, shininess);
    
    return saturate(diffuse + specular);
}

float4 PointLightDiffuseSpecular(float4 textureColor, float4 lightColor, float3 worldPosition, float3 normal, float3 viewDir)
{
    float4 diffuse = 0.0f;
    float4 specular = 0.0f;
    
    // Calculate the light attenuation.
    float distance = length(light.position - worldPosition);
    float attenuation = 1.0f / (light.constantAtt + light.linearAtt * distance + light.quadraticAtt * (distance * distance));
    
    // Calculate the diffuse lighting contribution for the fragment.
    float3 lightDir = normalize(light.position - worldPosition);
    float diffuseFactor = saturate(dot(normal, lightDir));
    diffuse = attenuation * textureColor * saturate(lightColor * diffuseFactor);
    
    // Calculate the specular lighting contribution for the fragment.
    float3 reflectDir = reflect(-lightDir, normal);
    float specularFactor = saturate(dot(reflectDir, viewDir));
    specular = attenuation * lightColor * specularTint * specularFactor * pow(specularFactor, shininess);
    
    return saturate(diffuse + specular);
}

float4 SpotLightDiffuseSpecular(float4 textureColor, float4 lightColor, float3 worldPosition, float3 normal, float3 viewDir)
{
    float4 diffuse = 0.0f;
    float4 specular = 0.0f;
        
    // Calculate the diffuse lighting factor for the fragment.
    float3 lightDir = normalize(light.position - worldPosition);
    float diffuseFactor = saturate(dot(normal, lightDir));
    
    // Calculate the intensity of the spot light.
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.innerCutOff - light.outercutoff;
    float intensity = saturate((theta - light.outercutoff) / epsilon);
    
    // Calculate the difuse lighting contribution for the fragment.
    diffuse = intensity * textureColor * saturate(lightColor * diffuseFactor);
    
    // Calculate the specular lighting contribution for the fragment.
    float3 reflectDir = reflect(-lightDir, normal);
    float specularFactor = saturate(dot(reflectDir, viewDir));
    specular = intensity * lightColor * specularTint * specularFactor * pow(specularFactor, shininess);
    
    return saturate(diffuse + specular);
}

/*==========
PIXEL SHADER
==========*/
float4 PixelMain(PixelInputType input) : SV_TARGET
{
    float4 textureColor = shaderTexture.Sample(sampleType, input.tex);
    float4 lightColor = float4(light.color.xyz * light.color.w, 1.0f);
    
    float3 normal = normalize(input.normal);
    float3 viewDir = normalize(input.viewDir);
        
    float4 color = 0.0f;
    
    if (light.type == DIRECTIONAL_LIGHT_TYPE) {
        color = DirectionalLightDiffuseSpecular(textureColor, lightColor, normal, viewDir);
    }
    else if (light.type == POINT_LIGHT_TYPE) {
        color = PointLightDiffuseSpecular(textureColor, lightColor, input.worldPos.xyz, normal, viewDir);
    }
    else if (light.type == SPOT_LIGHT_TYPE) {
        color = SpotLightDiffuseSpecular(textureColor, lightColor, input.worldPos.xyz, normal, viewDir);
    }
    
    return color;
}