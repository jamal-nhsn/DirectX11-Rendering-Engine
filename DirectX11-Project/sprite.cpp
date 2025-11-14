#include "sprite.h"

const ComponentId Sprite::ID = ComponentId::Sprite;

Sprite::Sprite(int entityId)
	: m_entityId(entityId)
{
}

int Sprite::GetEntityId()
{
	return m_entityId;
}

void Sprite::SetShader(Shader* shader)
{
	m_spriteData.shader = shader;
}

void Sprite::SetTexture(Engine::Texture2D* texture)
{
	m_spriteData.texture = texture;
}

void Sprite::SetSourceX(int sourceX)
{
	m_spriteData.sourceRect.x = static_cast<float>(sourceX);
}

void Sprite::SetSourceY(int sourceY)
{
	m_spriteData.sourceRect.y = static_cast<float>(sourceY);
}

void Sprite::SetWidth(int width)
{
	m_spriteData.sourceRect.z = static_cast<float>(width);
}

void Sprite::SetHeight(int height)
{
	m_spriteData.sourceRect.w = static_cast<float>(height);
}

void Sprite::SetTint(float red, float green, float blue, float alpha)
{
	m_spriteData.tint = { red, green, blue, alpha };
}

const SpriteData& Sprite::GetSpriteData()
{
	return m_spriteData;
}