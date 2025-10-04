/*====
SHARED
====*/
#ifndef LIGHT_H
#define LIGHT_H

#define DIRECTIONAL_LIGHT_TYPE 1.0f
#define POINT_LIGHT_TYPE 2.0f
#define SPOT_LIGHT_TYPE 3.0f

/*======
C++ SIDE
======*/
#if defined(__cplusplus)

/*======
INCLUDES
======*/
#include "components.h"
#include <directxmath.h>

struct LightData
{
	float type;
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT4 color;
	DirectX::XMFLOAT3 direction;
	float innerCutOff;
	float outercutoff;
	float constantAtten;
	float linearAtten;
	float quadraticAtten;
};

class Light
{
public:
	Light(int entityId);
	Light(const Light&);
	~Light();

	int GetEntityId();

	void SetType(float type);
	void SetColor(DirectX::XMFLOAT4 color);
	void SetInnerCutOffAngle(float deg);
	void SetOuterCutOffAngle(float deg);
	void SetConstantAttenuation(float constantAtten);
	void SetLinearAttenuation(float linearAtten);
	void SetQuadraticAttenuation(float quadraticAtten);

	float GetType();
	DirectX::XMFLOAT4 GetColor();
	float GetInnerCutOff();
	float GetOuterCutOff();
	float GetInnerCutOffAngle();
	float GetOuterCutOffAngle();
	float GetConstantAttenuation();
	float GetLinearAttenuation();
	float GetQuadraticAttenuation();

	void FillLightData(LightData* lightData, DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 direction);

public:
	static const ComponentId ID;

private:
	int m_entityId;

	float m_type;
	DirectX::XMFLOAT4 m_color;
	float m_innerCutOff;
	float m_outerCutOff;
	float m_constantAtten;
	float m_linearAtten;
	float m_quadraticAtten;
};

/*=======
HLSL SIDE
=======*/
#else

struct LightData
{
	float  type;
	float3 position;
	float4 color;
	float3 direction;
	float  innerCutOff;
	float  outercutoff;
	float  constantAtt;
	float  linearAtt;
	float  quadraticAtt;
};

#endif
#endif