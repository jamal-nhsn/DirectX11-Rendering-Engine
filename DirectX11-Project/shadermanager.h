#pragma once

/*======
INCLUDES
======*/
#include <d3d11.h>
#include <unordered_map>
#include <typeindex>

#include "shader.h"
#include "colorshader.h"
#include "textureshader.h"
#include "defaultlightshader.h"
#include "defaultbaseshader.h"
#include "defaultspriteshader.h"

class ShaderManager
{
public:
	ShaderManager();
	ShaderManager(const ShaderManager&);
	~ShaderManager();

	bool Initialize(ID3D11Device* device, HWND hwnd);
	void Shutdown();

	template<typename ShaderType>
	ShaderType* GetShader();

private:
	std::unordered_map<std::type_index, Shader*> m_shaderBank;
};

template<typename ShaderType>
ShaderType* ShaderManager::GetShader()
{
	return (ShaderType*)m_shaderBank[typeid(ShaderType)];
}