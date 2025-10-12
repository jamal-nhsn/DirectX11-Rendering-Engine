#pragma once

/*======
INCLUDES
======*/
#include <directxmath.h>

struct Vertex3D
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 normal;
	DirectX::XMFLOAT2 texCoord;
	DirectX::XMFLOAT3 tangent;
	DirectX::XMFLOAT4 color;
};

struct Vertex2D
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT2 texCoord;
	DirectX::XMFLOAT4 color;
	float samplerIndex;
	float textureIndex;
};