/*====
SHARED
====*/
#pragma once

#define DIRECTIONAL_LIGHT_TYPE 0.0f

/*======
C++ SIDE
======*/
#if defined(__cplusplus)

/*======
INCLUDES
======*/
#include "components.h"
#include <directxmath.h>

struct LightStruct
{
	DirectX::XMFLOAT4 position;
	DirectX::XMFLOAT4 color;
	DirectX::XMFLOAT3 direction;
	float type;
};


class Light
{
public:
	Light(int entityId);
	Light(const Light&);
	~Light();

	int GetEntityId();

	void SetColor(DirectX::XMFLOAT4 color);
	void SetType(int type);

	DirectX::XMFLOAT4 GetColor();
	float GetType();

public:
	static const ComponentId ID;

private:
	int m_entityId;

	DirectX::XMFLOAT4 m_color;
	float m_type;
};

/*=======
HLSL SIDE
=======*/
#else

struct Light
{
	float4 position;
	float4 color;
	float3 direction;
	float type;
};

#endif