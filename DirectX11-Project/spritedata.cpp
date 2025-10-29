#include "spritedata.h"
#include "renderer2d.h"

SpriteData::SpriteData(Renderer2D* renderer2D)
	: m_renderer2D(renderer2D)
	, m_modelMatrix(0)
	, m_destRect(0.0f, 0.0f, 1.0f, 1.0f)
	, m_sourceRect(0.0f, 0.0f, 0.0f, 0.0f)
	, m_tint(1.0f, 1.0f, 1.0f, 1.0f)
	, m_rotationQuaternion(DirectX::XMQuaternionIdentity())
{
}

SpriteData& SpriteData::WithModelMatrix(const DirectX::XMMATRIX* modelMatrix)
{
	m_modelMatrix = modelMatrix;

	return *this;
}

SpriteData& SpriteData::WithDestCenterPosition(float x, float y)
{
	m_destRect.x = x;
	m_destRect.y = y;

	return *this;
}

SpriteData& SpriteData::WithDestDimensions(float width, float height)
{
	m_destRect.z = width;
	m_destRect.w = height;

	return *this;
}

SpriteData& SpriteData::WithRotationDeg(float deg)
{
	m_rotationQuaternion = DirectX::XMQuaternionRotationAxis(
		{ 0.0f, 0.0f, 1.0f, 0.0f },
		DirectX::XMConvertToRadians(deg)
	);

	return *this;
}

SpriteData& SpriteData::WithRotationQuaternion(DirectX::XMVECTOR quaternion)
{
	m_rotationQuaternion = quaternion;

	return *this;
}

SpriteData& SpriteData::WithSourceOrigin(int x, int y)
{
	m_sourceRect.x = static_cast<float>(x);
	m_sourceRect.y = static_cast<float>(y);

	return *this;
}

SpriteData& SpriteData::WithSourceDimensions(int width, int height)
{
	m_sourceRect.z = static_cast<float>(width);
	m_sourceRect.w = static_cast<float>(height);

	return *this;
}

SpriteData& SpriteData::WithTint(DirectX::XMFLOAT4 tint)
{
	m_tint = tint;

	return *this;
}

void SpriteData::Submit(Shader* shader, Texture* texture)
{
	if (!shader || !texture) return;

	m_sourceRect.z = m_sourceRect.z == 0 ? texture->GetWidth() : m_sourceRect.z;
	m_sourceRect.w = m_sourceRect.w == 0 ? texture->GetHeight() : m_sourceRect.w;

	if (!m_modelMatrix) {
		DirectX::XMMATRIX modelMatrix =
			DirectX::XMMatrixScaling(m_destRect.z, m_destRect.z, 1.0f) *
			DirectX::XMMatrixRotationQuaternion(m_rotationQuaternion) *
			DirectX::XMMatrixTranslation(m_destRect.x, m_destRect.z, 0.0f);

		m_modelMatrix = &modelMatrix;
		m_renderer2D->SubmitSprite(shader, texture, *this);
	}
	else {
		m_renderer2D->SubmitSprite(shader, texture, *this);
	}
}