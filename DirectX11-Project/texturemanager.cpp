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
	// Create stoneWall texture from targa image.
	TargaLoader targaLoader;
	Texture* texture = targaLoader.LoadTexture("../DirectX11-Project/Textures/stoneWall.tga", device, deviceContext);
	if (!texture) {
		return false;
	}
	// Load the sampler description from the texture meta data.
	TextureMetaLoader textureMetaLoader;
	D3D11_SAMPLER_DESC samplerDesc = textureMetaLoader.LoadSamplerSettings("../DirectX11-Project/Textures/stoneWall.texturemeta", device, deviceContext);
	// Create the texture sampler state if it doesn't exist.
	if (m_samplerBank.find(samplerDesc) == m_samplerBank.end()) {
		ID3D11SamplerState* samplerState;
		HRESULT result = device->CreateSamplerState(&samplerDesc, &samplerState);
		m_samplerBank[samplerDesc] = FAILED(result) ? 0 : samplerState;
	}
	// Set the sampler and register the texture.
	texture->SetSamplerState(m_samplerBank[samplerDesc]);
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
	for (auto& entry : m_samplerBank) {
		entry.second->Release();
	}
}