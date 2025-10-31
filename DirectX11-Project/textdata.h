#pragma once

/*======
INCLUDES
======*/
#include <directxmath.h>
#include <string.h>

#include "shader.h"
#include "texture.h"

struct TextData
{
public:
	TextData()
		: shader(0)
		, texture(0)
		, text("")
		, tint(1.0f, 1.0f, 1.0f, 1.0f)
	{
	}
	TextData(const TextData& other) = default;
	~TextData() = default;

public:
	Shader* shader;
	Texture* texture;
	std::string text;
	DirectX::XMFLOAT4 tint;
};