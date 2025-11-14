#pragma once

/*======
INCLUDES
======*/
#include "components.h"
#include "src/Renderer/mesh.h"
#include "src/Renderer/texture2d.h"
#include "shader.h"

class Model
{
public:
	Model(int entityId);
	Model(const Model&) = default;
	~Model() = default;

	int GetEntityId();

	void SetMesh(Engine::Mesh* mesh);
	void SetBaseShader(Shader* shader);
	void SetLightShader(Shader* shader);
	void SetTexture(Engine::Texture2D* texture);

	void SetSpecularTint(DirectX::XMFLOAT4 specularTint);
	void SetShininess(float shininess);

	Engine::Mesh* GetMesh();
	Shader*  GetBaseShader();
	Shader*  GetLightShader();
	Engine::Texture2D* GetTexture();

	DirectX::XMFLOAT4 GetSpecularTint();
	float             GetShininess();

public:
	static const ComponentId ID;

private:
	int m_entityId;

	Engine::Mesh* m_mesh;
	Shader*  m_baseShader;
	Shader*  m_lightShader;
	Engine::Texture2D* m_texture;

	DirectX::XMFLOAT4 m_specularTint;
	float			  m_shininess;
};