#include "directionallight.h"

const ComponentId DirectionalLight::ID = ComponentId::DirectionalLight;

DirectionalLight::DirectionalLight(int entityId)
	: m_entityId(entityId)
{
	m_color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
}

DirectionalLight::DirectionalLight(const DirectionalLight& other)
{
	m_entityId = other.m_entityId;
	m_color = other.m_color;
}

DirectionalLight::~DirectionalLight()
{
}

int DirectionalLight::GetEntityId()
{
	return m_entityId;
}

void DirectionalLight::SetColor(DirectX::XMFLOAT4 color)
{
	m_color = color;
}

DirectX::XMFLOAT4 DirectionalLight::GetColor()
{
	return m_color;
}