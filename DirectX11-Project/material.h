#pragma once

/*======
INCLUDES
======*/
#include <d3d11.h>
#include "texture.h"

class Shader;

class Material
{
public:
	Material();
	Material(const Material&);
	~Material();

private:
	Shader* m_shader;
	Texture* m_texture;

public:
	void SetShader(Shader* shader);
	void SetTexture(Texture* texture);

	Shader* GetShader();
	Texture* GetTexture();
};