#include "model.h"

const ComponentId Model::ID = ComponentId::Model;

Model::Model(int entityId)
	: m_entityId(entityId)
{
	m_mesh = 0;
	m_texture = 0;
	m_shader = 0;
}

Model::Model(const Model& other)
{
	m_entityId = other.m_entityId;
	m_mesh = other.m_mesh;
	m_texture = other.m_texture;
	m_shader = other.m_shader;
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

void Model::SetTexture(Texture* texture)
{
	m_texture = texture;
}

void Model::SetShader(Shader* shader)
{
	m_shader = shader;
}

Mesh* Model::GetMesh()
{
	return m_mesh;
}

Texture* Model::GetTexture()
{
	return m_texture;
}

Shader* Model::GetShader()
{
	return m_shader;
}