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
	TargaLoader targaLoader;
	Texture* texture;

	// Create stoneWall texture.
	texture = targaLoader.LoadTexture("../DirectX11-Project/Textures/stoneWall.tga", device, deviceContext);
	if (!texture) {
		return false;
	}
	m_textureBank["stoneWall"] = texture;

	return true;
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