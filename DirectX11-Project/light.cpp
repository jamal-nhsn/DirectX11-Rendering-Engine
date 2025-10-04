#include "light.h"

const ComponentId Light::ID = ComponentId::Light;

Light::Light(int entityId)
	: m_entityId(entityId)
{
	m_type = DIRECTIONAL_LIGHT_TYPE;
	m_color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetInnerCutOffAngle(12.0f);
	SetOuterCutOffAngle(36.0f);
	m_constantAtten  = 1.0f;
	m_linearAtten    = 0.09f;
	m_quadraticAtten = 0.032f;
}

Light::Light(const Light& other)
{
	m_entityId		   = other.m_entityId;
	m_type			   = other.m_type;
	m_color			   = other.m_color;
	m_innerCutOff      = other.m_innerCutOff;
	m_outerCutOff      = other.m_outerCutOff;
	m_constantAtten    = other.m_constantAtten;
	m_linearAtten      = other.m_linearAtten;
	m_quadraticAtten   = other.m_quadraticAtten;
}

Light::~Light()
{
}

int Light::GetEntityId()
{
	return m_entityId;
}

void Light::SetType(float type)
{
	m_type = type;
}

void Light::SetColor(DirectX::XMFLOAT4 color)
{
	m_color = color;
}

void Light::SetInnerCutOffAngle(float deg)
{
	m_innerCutOff = DirectX::XMScalarCos(DirectX::XMConvertToRadians(deg));
}

void Light::SetOuterCutOffAngle(float deg)
{
	m_outerCutOff = DirectX::XMScalarCos(DirectX::XMConvertToRadians(deg));
}

void Light::SetConstantAttenuation(float constantAtten)
{
	m_constantAtten = constantAtten;
}

void Light::SetLinearAttenuation(float linearAtten)
{
	m_linearAtten = linearAtten;
}

void Light::SetQuadraticAttenuation(float quadraticAtten)
{
	m_quadraticAtten = quadraticAtten;
}

float Light::GetType()
{
	return m_type;
}

DirectX::XMFLOAT4 Light::GetColor()
{
	return m_color;
}

float Light::GetInnerCutOff()
{
	return m_innerCutOff;
}

float Light::GetOuterCutOff()
{
	return m_outerCutOff;
}

float Light::GetInnerCutOffAngle()
{
	return DirectX::XMConvertToDegrees(DirectX::XMScalarACos(m_innerCutOff));
}

float Light::GetOuterCutOffAngle()
{
	return DirectX::XMConvertToDegrees(DirectX::XMScalarACos(m_outerCutOff));
}

float Light::GetConstantAttenuation()
{
	return m_constantAtten;
}

float Light::GetLinearAttenuation()
{
	return m_linearAtten;
}

float Light::GetQuadraticAttenuation()
{
	return m_quadraticAtten;
}

void Light::FillLightData(LightData* lightData, DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 direction)
{
	lightData->type           = m_type;
	lightData->position       = position;
	lightData->color          = m_color;
	lightData->direction      = direction;
	lightData->innerCutOff    = m_innerCutOff;
	lightData->outercutoff    = m_outerCutOff;
	lightData->constantAtten  = m_constantAtten;
	lightData->linearAtten    = m_linearAtten;
	lightData->quadraticAtten = m_quadraticAtten;	
}