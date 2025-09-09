#include "texturemanager.h"

TextureManager::TextureManager()
{
}

TextureManager::TextureManager(const TextureManager& other)
{
}

TextureManager::~TextureManager()
{
}

bool TextureManager::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	Texture* texture;
	bool success = false;

	// Create stoneWallTexture
	texture = new Texture();
	success = texture->Initialize(device, deviceContext, "../DirectX11-Project/Textures/stoneWall.tga");
	if (!success) {
		delete texture;
		return success;
	}
	m_textureBank["stoneWall"] = texture;

	return success;
}

Texture* TextureManager::GetTexture(const char* textureName)
{
	return m_textureBank[textureName];
}

void TextureManager::Shutdown()
{
	for (auto& entry : m_textureBank) {
		entry.second->Shutdown();
		delete entry.second;
	}
}