#pragma once

/*======
INCLUDES
======*/
#include <unordered_map>

#include "targaloader.h"

class TextureManager
{
public:
	TextureManager();
	TextureManager(const TextureManager&);
	~TextureManager();

	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	Texture* GetTexture(const char* textureName);
	void Shutdown();

private:
	std::unordered_map<const char*, Texture*> m_textureBank;
};