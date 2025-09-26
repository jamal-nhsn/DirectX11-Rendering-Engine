#include "shadermanager.h"

ShaderManager::ShaderManager()
{
}

ShaderManager::ShaderManager(const ShaderManager& other)
{
}

ShaderManager::~ShaderManager()
{
}

bool ShaderManager::Initialize(ID3D11Device* device, HWND hwnd)
{
	bool success = false;

	ColorShader* colorShader = new ColorShader;
	success = colorShader->Initialize(device, hwnd);
	if (!success) {
		delete colorShader;
		return success;
	}
	m_shaderBank[typeid(ColorShader)] = colorShader;

	TextureShader* textureShader = new TextureShader;
	success = textureShader->Initialize(device, hwnd);
	if (!success) {
		delete textureShader;
		return success;
	}
	m_shaderBank[typeid(TextureShader)] = textureShader;

	DirLightShader* dirLightShader = new DirLightShader;
	success = dirLightShader->Initialize(device, hwnd);
	if (!success) {
		delete dirLightShader;
		return success;
	}
	m_shaderBank[typeid(DirLightShader)] = dirLightShader;

	return success;
}

void ShaderManager::Shutdown()
{
	for (auto& entry : m_shaderBank) {
		entry.second->Shutdown();
		delete entry.second;
	}
}

