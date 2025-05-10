#pragma once

/*======
INCLUDES
======*/
#include "shader.h"

class Material
{
public:
	Material();
	Material(const Material&);
	~Material();

	bool Initialize(Shader* shader);
	void Bind(ID3D11DeviceContext* deviceContext);

private:
	Shader* m_shader;
};