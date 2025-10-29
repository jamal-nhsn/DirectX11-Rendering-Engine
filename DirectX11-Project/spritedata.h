#pragma once

class Renderer2D;

/*======
INCLUDES
======*/
#include <directxmath.h>

#include "shader.h"
#include "texture.h"

class SpriteData
{
private:
	SpriteData(Renderer2D* renderer2D);
public:
	SpriteData(const SpriteData& other) = default;
	~SpriteData() = default;

	SpriteData& WithModelMatrix(const DirectX::XMMATRIX* modelMatrix);
	SpriteData& WithDestCenterPosition(float x, float y);
	SpriteData& WithDestDimensions(float width, float height);
	SpriteData& WithRotationDeg(float deg);
	SpriteData& WithRotationQuaternion(DirectX::XMVECTOR quaternion);
	SpriteData& WithSourceOrigin(int x, int y);
	SpriteData& WithSourceDimensions(int width, int height);
	SpriteData& WithTint(DirectX::XMFLOAT4 tint);
	void Submit(Shader* shader, Texture* texture);

private:
	Renderer2D* m_renderer2D;
	const DirectX::XMMATRIX* m_modelMatrix;

	DirectX::XMFLOAT4 m_destRect;
	DirectX::XMFLOAT4 m_sourceRect;
	DirectX::XMFLOAT4 m_tint;
	DirectX::XMVECTOR m_rotationQuaternion;

	friend Renderer2D;
};