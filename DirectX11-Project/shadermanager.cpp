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

	DefaultBaseShader* defaultBaseShader = new DefaultBaseShader;
	success = defaultBaseShader->Initialize(device, hwnd);
	if (!success) {
		delete defaultBaseShader;
		return success;
	}
	m_shaderBank[typeid(DefaultBaseShader)] = defaultBaseShader;

	DefaultLightShader* defaultLightShader = new DefaultLightShader;
	success = defaultLightShader->Initialize(device, hwnd);
	if (!success) {
		delete defaultLightShader;
		return success;
	}
	m_shaderBank[typeid(DefaultLightShader)] = defaultLightShader;

	DefaultSpriteShader* defaultSpriteShader = new DefaultSpriteShader;
	success = defaultSpriteShader->Initialize(device, hwnd);
	if (!success) {
		delete defaultSpriteShader;
		return success;
	}
	m_shaderBank[typeid(DefaultSpriteShader)] = defaultSpriteShader;

	return success;
}

void ShaderManager::Shutdown()
{
	for (auto& entry : m_shaderBank) {
		entry.second->Shutdown();
		delete entry.second;
	}
}

