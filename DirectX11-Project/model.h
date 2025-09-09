#pragma once

/*======
INCLUDES
======*/
#include "components.h"
#include "mesh.h"
#include "texture.h"
#include "shader.h"

class Model
{
public:
	Model(int entityId);
	Model(const Model&);
	~Model();

	int GetEntityId();

	void SetMesh(Mesh* mesh);
	void SetTexture(Texture* texture);
	void SetShader(Shader* shader);

	Mesh* GetMesh();
	Texture* GetTexture();
	Shader* GetShader();

public:
	static const ComponentId ID;

private:
	int m_entityId;

	Mesh* m_mesh;
	Texture* m_texture;
	Shader* m_shader;
};