#include "model.h"

const ComponentId Model::ID = ComponentId::Model;

Model::Model(int entityId)
	: m_entityId(entityId)
{
	m_mesh    = 0;
	m_shader  = 0;
	m_texture = 0;
}

Model::Model(const Model& other)
{
	m_entityId = other.m_entityId;
	m_mesh     = other.m_mesh;
	m_shader   = other.m_shader;
	m_texture  = other.m_texture;
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

void Model::SetShader(Shader* shader)
{
	m_shader = shader;
}

void Model::SetTexture(Texture* texture)
{
	m_texture = texture;
}

Mesh* Model::GetMesh()
{
	return m_mesh;
}

Shader* Model::GetShader()
{
	return m_shader;
}

Texture* Model::GetTexture()
{
	return m_texture;
}
