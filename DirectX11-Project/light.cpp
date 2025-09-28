#include "light.h"

const ComponentId Light::ID = ComponentId::Light;

Light::Light(int entityId)
	: m_entityId(entityId)
{
	m_color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_type =  DIRECTIONAL_LIGHT_TYPE;
}

Light::Light(const Light& other)
{
	m_entityId = other.m_entityId;
	m_color    = other.m_color;
	m_type     = other.m_type;
}

Light::~Light()
{
}

int Light::GetEntityId()
{
	return m_entityId;
}

void Light::SetColor(DirectX::XMFLOAT4 color)
{
	m_color = color;
}

DirectX::XMFLOAT4 Light::GetColor()
{
	return m_color;
}

void Light::SetType(int type)
{
	m_type = type;
}

float Light::GetType()
{
	return m_type;
}