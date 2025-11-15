#pragma once

#include <directxmath.h>

namespace Engine
{
	struct Vertex3D
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT2 texCoord;
		DirectX::XMFLOAT3 tangent;
		DirectX::XMFLOAT4 color;

		Vertex3D(
			DirectX::XMFLOAT3 position = DirectX::XMFLOAT3{ 0.0f, 0.0f, 0.0f },
			DirectX::XMFLOAT3 normal = DirectX::XMFLOAT3{ 0.0f, 0.0f, 0.0f },
			DirectX::XMFLOAT2 texCoord = DirectX::XMFLOAT2{ 0.0f, 0.0f },
			DirectX::XMFLOAT3 tangent = DirectX::XMFLOAT3{ 0.0f, 0.0f, 0.0f },
			DirectX::XMFLOAT4 color = DirectX::XMFLOAT4{ 1.0f, 1.0f, 1.0f, 1.0f }
		)
			: position(position), normal(normal), texCoord(texCoord), tangent(tangent), color(color)
		{
		}
	};

	struct Vertex2D
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT2 texCoord;
		DirectX::XMFLOAT4 color;

		Vertex2D(
			DirectX::XMFLOAT3 position = DirectX::XMFLOAT3{ 0.0f, 0.0f, 0.0f },
			DirectX::XMFLOAT2 texCoord = DirectX::XMFLOAT2{ 0.0f, 0.0f },
			DirectX::XMFLOAT4 color = DirectX::XMFLOAT4{ 1.0f, 1.0f, 1.0f, 1.0f }
		)
			: position(position), texCoord(texCoord), color(color)
		{
		}
	};
}