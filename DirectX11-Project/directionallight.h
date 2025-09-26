#pragma once

/*======
INCLUDES
======*/
#include "components.h"
#include <directxmath.h>

class DirectionalLight
{
public:
	DirectionalLight(int entityId);
	DirectionalLight(const DirectionalLight&);
	~DirectionalLight();

	int GetEntityId();

	void SetColor(DirectX::XMFLOAT4 color);

	DirectX::XMFLOAT4 GetColor();

public:
	static const ComponentId ID;

private:
	int m_entityId;

	DirectX::XMFLOAT4 m_color;
};