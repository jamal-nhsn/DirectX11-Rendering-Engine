#include "material.h"

Material::Material()
{
	m_shader = 0;
}

Material::Material(const Material& other)
{
}

Material::~Material()
{
}

bool Material::Initialize(Shader* shader)
{
	m_shader = shader;
	return m_shader != 0;
}

void Material::Bind(ID3D11DeviceContext* deviceContext)
{
	m_shader->Bind(deviceContext);
}