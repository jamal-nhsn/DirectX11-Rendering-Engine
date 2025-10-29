#pragma once

/*======
INCLUDES
======*/
#include <directxmath.h>

#include "shader.h"
#include "texture.h"

struct SpriteData
{
public:
	SpriteData()
		: shader(0)
		, texture(0)
		, sourceRect(0.0f, 0.0f, 0.0f, 0.0f)
		, tint(1.0f, 1.0f, 1.0f, 1.0f)
	{
	}
	SpriteData(const SpriteData& other) = default;
	~SpriteData() = default;

public:
	Shader* shader;
	Texture* texture;
	DirectX::XMFLOAT4 sourceRect;
	DirectX::XMFLOAT4 tint;
};