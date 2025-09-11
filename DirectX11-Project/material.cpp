#include "material.h"

Material::Material()
{
	m_shader = 0;
	m_texture = 0;
}

Material::Material(const Material& other)
{
	m_shader = other.m_shader;
	m_texture = other.m_texture;
}

Material::~Material()
{
}

void Material::SetShader(Shader* shader)
{
	m_shader = shader;
}

void Material::SetTexture(Texture* texture)
{
	m_texture = texture;
}

Shader* Material::GetShader() {
	return m_shader;
}

Texture* Material::GetTexture() {
	return m_texture;
}
