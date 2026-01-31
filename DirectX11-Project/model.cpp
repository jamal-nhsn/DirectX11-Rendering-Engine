#include "pch.h"
#include "model.h"

const ComponentId Model::ID = ComponentId::Model;

Model::Model(int entityId)
	: m_entityId(entityId)
{
	m_mesh        = 0;
	m_baseShader  = 0;
	m_lightShader = 0;
	m_texture     = 0;

	m_specularTint = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_shininess    = 32.0f;
}

int Model::GetEntityId()
{
	return m_entityId;
}

void Model::SetMesh(Engine::Mesh* mesh)
{
	m_mesh = mesh;
}

void Model::SetBaseShader(Shader* shader)
{
	m_baseShader = shader;
}

void Model::SetLightShader(Shader* shader)
{
	m_lightShader = shader;
}

void Model::SetTexture(Engine::Texture2D* texture)
{
	m_texture = texture;
}

void Model::SetSpecularTint(DirectX::XMFLOAT4 specularTint)
{
	m_specularTint = specularTint;
}

void Model::SetShininess(float shininess)
{
	m_shininess = shininess;
}

Engine::Mesh* Model::GetMesh()
{
	return m_mesh;
}

Shader* Model::GetBaseShader()
{
	return m_baseShader;
}

Shader* Model::GetLightShader()
{
	return m_lightShader;
}

Engine::Texture2D* Model::GetTexture()
{
	return m_texture;
}

DirectX::XMFLOAT4 Model::GetSpecularTint()
{
	return m_specularTint;
}

float Model::GetShininess()
{
	return m_shininess;
}
