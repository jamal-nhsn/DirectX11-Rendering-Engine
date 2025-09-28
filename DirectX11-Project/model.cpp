#include "model.h"

const ComponentId Model::ID = ComponentId::Model;

Model::Model(int entityId)
	: m_entityId(entityId)
{
	m_mesh        = 0;
	m_baseShader  = 0;
	m_lightShader = 0;
	m_texture     = 0;
}

Model::Model(const Model& other)
{
	m_entityId     = other.m_entityId;
	m_mesh         = other.m_mesh;
	m_baseShader   = other.m_baseShader;
	m_lightShader  = other.m_lightShader;
	m_texture      = other.m_texture;
}

Model::~Model()
{
}

int Model::GetEntityId()
{
	return m_entityId;
}

void Model::SetMesh(Mesh* mesh)
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

void Model::SetTexture(Texture* texture)
{
	m_texture = texture;
}

Mesh* Model::GetMesh()
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

Texture* Model::GetTexture()
{
	return m_texture;
}
