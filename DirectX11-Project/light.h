/*====
SHARED
====*/
#pragma once

#define DIRECTIONAL_LIGHT_TYPE 0

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
	DirectX::XMFLOAT3 direction;
	DirectX::XMFLOAT4 color;
	int type;
	DirectX::XMFLOAT4 padding;
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
	int GetType();

public:
	static const ComponentId ID;

private:
	int m_entityId;

	DirectX::XMFLOAT4 m_color;
	int m_type;
};

/*=======
HLSL SIDE
=======*/
#else

struct Light
{
	float4 position;
	float3 direction;
	float4 color;
	int type;
	float4 padding;
};

#endif