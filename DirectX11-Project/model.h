#pragma once

/*======
INCLUDES
======*/
#include "components.h"
#include "src/Renderer/mesh.h"
#include "texture.h"
#include "shader.h"

class Model
{
public:
	Model(int entityId);
	Model(const Model&) = default;
	~Model() = default;

	int GetEntityId();

	void SetMesh(std::shared_ptr<Engine::Mesh> mesh);
	void SetBaseShader(Shader* shader);
	void SetLightShader(Shader* shader);
	void SetTexture(Texture* texture);

	void SetSpecularTint(DirectX::XMFLOAT4 specularTint);
	void SetShininess(float shininess);

	std::shared_ptr<Engine::Mesh> GetMesh();
	Shader*  GetBaseShader();
	Shader*  GetLightShader();
	Texture* GetTexture();

	DirectX::XMFLOAT4 GetSpecularTint();
	float             GetShininess();

public:
	static const ComponentId ID;

private:
	int m_entityId;

	std::shared_ptr<Engine::Mesh> m_mesh;
	Shader*  m_baseShader;
	Shader*  m_lightShader;
	Texture* m_texture;

	DirectX::XMFLOAT4 m_specularTint;
	float			  m_shininess;
};