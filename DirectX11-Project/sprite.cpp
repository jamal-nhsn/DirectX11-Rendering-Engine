#include "sprite.h"

const ComponentId Sprite::ID = ComponentId::Sprite;

Sprite::Sprite(int entityId)
	: m_entityId(entityId)
{
	m_shader = 0;
	m_texture = 0;
	m_tint = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_width = 1;
	m_height = 1;
	m_sourceX = 0;
	m_sourceY = 0;
}

Sprite::Sprite(const Sprite& other)
{
	m_entityId = other.m_entityId;
	m_shader = other.m_shader;
	m_texture = other.m_texture;
	m_tint = other.m_tint;
	m_width = other.m_width;
	m_height = other.m_height;
	m_sourceX = other.m_sourceX;
	m_sourceY = other.m_sourceY;
}

Sprite::~Sprite()
{
}

int Sprite::GetEntityId()
{
	return m_entityId;
}

void Sprite::SetShader(Shader* shader)
{
	m_shader = shader;
}
void Sprite::SetTexture(Texture* texture)
{
	m_texture = texture;
}

Shader* Sprite::GetShader()
{
	return m_shader;
}
Texture* Sprite::GetTexture()
{
	return m_texture;
}

void Sprite::SetTint(float red, float green, float blue, float alpha)
{
	m_tint.x = red;
	m_tint.y = green;
	m_tint.z = blue;
	m_tint.w = alpha;
}
void Sprite::SetWidth(int width)
{
	if (width == 0) {
		m_width = 1;
	}
	m_width = width;
}
void Sprite::SetHeight(int height)
{
	if (height == 0) {
		m_height = 1;
	}
	m_height = height;
}
void Sprite::SetSourceX(int sourceX)
{
	m_sourceX = sourceX;
}
void Sprite::SetSourceY(int sourceY)
{
	m_sourceY = sourceY;
}

DirectX::XMFLOAT4 Sprite::GetUVBounds()
{
	float x1 = static_cast<float>(m_sourceX) / static_cast<float>(m_texture->GetWidth());
	float y1 = 1.0f - static_cast<float>(m_sourceY) / static_cast<float>(m_texture->GetHeight());

	float x2 = static_cast<float>(m_sourceX + m_width ) / static_cast<float>(m_texture->GetWidth());
	float y2 = 1.0f - static_cast<float>(m_sourceY + m_height) / static_cast<float>(m_texture->GetHeight());

	return DirectX::XMFLOAT4(x1, y1, x2, y2);
}
DirectX::XMFLOAT4 Sprite::GetTint()
{
	return m_tint;
}
int Sprite::GetWidth()
{
	return m_width;
}
int Sprite::GetHeight()
{
	return m_height;
}
int Sprite::GetSourceX()
{
	return m_sourceX;
}
int Sprite::GetSourceY()
{
	return m_sourceY;
}