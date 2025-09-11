#pragma once

/*======
INCLUDES
======*/
#include "components.h"
#include "mesh.h"
#include "material.h"

class Model
{
public:
	Model(int entityId);
	Model(const Model&);
	~Model();

	int GetEntityId();

	void SetMesh(Mesh* mesh);
	void SetMaterial(Material* material);

	Mesh* GetMesh();
	Material* GetMaterial();

public:
	static const ComponentId ID;

private:
	int m_entityId;

	Mesh* m_mesh;
	Material* m_material;
};