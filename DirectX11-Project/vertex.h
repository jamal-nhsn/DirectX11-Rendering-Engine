#pragma once

/*======
INCLUDES
======*/
#include <directxmath.h>

struct Vertex
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 normal;
	DirectX::XMFLOAT2 texCoord;
	DirectX::XMFLOAT3 tangent;
	DirectX::XMFLOAT4 color;
	float padding;
};