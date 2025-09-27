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
	void SetShader(Shader* shader);
	void SetTexture(Texture* texture);

	Mesh*    GetMesh();
	Shader*  GetShader();
	Texture* GetTexture();

public:
	static const ComponentId ID;

private:
	int m_entityId;

	Mesh*    m_mesh;
	Shader*  m_shader;
	Texture* m_texture;
};