#pragma once

/*======
INCLUDES
======*/
#include "components.h"
#include "texture.h"
#include "shader.h"

class Sprite
{
public:
	Sprite(int entityId);
	Sprite(const Sprite&);
	~Sprite();

	int GetEntityId();

	void SetShader(Shader* shader);
	void SetTexture(Texture* texture);

	Shader* GetShader();
	Texture* GetTexture();

	void SetColor(float red, float green, float blue, float alpha);
	void SetWidth(int width);
	void SetHeight(int height);
	void SetSourceX(int sourceX);
	void SetSourceY(int sourceY);

	DirectX::XMFLOAT4 GetUVBounds();
	DirectX::XMFLOAT4 GetTint();
	int GetWidth();
	int GetHeight();
	int GetSourceX();
	int GetSourceY();

public:
	static const ComponentId ID;

private:
	int m_entityId;

	Shader* m_shader;
	Texture* m_texture;

	DirectX::XMFLOAT4 m_tint;
	int m_width;
	int m_height;
	int m_sourceX;
	int m_sourceY;
};