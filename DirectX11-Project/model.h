#pragma once

/*======
INCLUDES
======*/
#include "components.h"
#include "mesh.h"
#include "material.h"
#include "shader.h"

class Model
{
public:
	Model(int entityId);
	Model(const Model&);
	~Model();

	int GetEntityId();

	void SetMesh(Mesh* mesh);
	void SetMaterial(Material* material);
	void SetShader(Shader* shader);

	Mesh* GetMesh();
	Material* GetMaterial();
	Shader* GetShader();

public:
	static const ComponentId ID;

private:
	int m_entityId;

	Mesh* m_mesh;
	Material* m_material;
	Shader* m_shader;
};