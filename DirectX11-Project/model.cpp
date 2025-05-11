#include "model.h"

const ComponentId Model::ID = ComponentId::Model;

Model::Model(int entityId)
	: m_entityId(entityId)
{
	m_mesh = 0;
	m_material = 0;
	m_shader = 0;
}

Model::Model(const Model& other)
{
	m_entityId = other.m_entityId;
	m_mesh = other.m_mesh;
	m_material = other.m_material;
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

void Model::SetMaterial(Material* material)
{
	m_material = material;
}

void Model::SetShader(Shader* shader)
{
	m_shader = shader;
}

Mesh* Model::GetMesh()
{
	return m_mesh;
}

Material* Model::GetMaterial()
{
	return m_material;
}

Shader* Model::GetShader()
{
	return m_shader;
}