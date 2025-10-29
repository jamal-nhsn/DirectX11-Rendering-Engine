#pragma once

/*======
INCLUDES
======*/
#include "components.h"
#include "texture.h"
#include "shader.h"
#include "spritedata.h"

class Sprite
{
public:
	Sprite(int entityId);
	Sprite(const Sprite&) = default;
	~Sprite() = default;

	int GetEntityId();

	void SetShader(Shader* shader);
	void SetTexture(Texture* texture);
	void SetSourceX(int sourceX);
	void SetSourceY(int sourceY);
	void SetWidth(int width);
	void SetHeight(int height);
	void SetTint(float red, float green, float blue, float alpha);

	const SpriteData& GetSpriteData();

public:
	static const ComponentId ID;

private:
	int m_entityId;
	SpriteData m_spriteData;
};