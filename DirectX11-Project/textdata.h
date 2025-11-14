#pragma once

/*======
INCLUDES
======*/
#include <directxmath.h>
#include <string.h>

#include "shader.h"
#include "src/Renderer/texture2d.h"

struct TextData
{
public:
	TextData()
		: shader(0)
		, texture(0)
		, text("")
		, characterDimensions(0.0f, 0.0f)
		, tint(1.0f, 1.0f, 1.0f, 1.0f)
	{
	}
	TextData(const TextData& other) = default;
	~TextData() = default;

public:
	Shader* shader;
	Engine::Texture2D* texture;
	std::string text;
	DirectX::XMFLOAT2 characterDimensions;
	DirectX::XMFLOAT4 tint;
};