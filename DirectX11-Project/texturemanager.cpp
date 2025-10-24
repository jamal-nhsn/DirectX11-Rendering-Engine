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
	bool success = LoadTexture(
		device,
		deviceContext,
		"../DirectX11-Project/Textures/stoneWall.tga",
		"../DirectX11-Project/Textures/stoneWall.texturemeta",
		"stoneWall"
	) && LoadTexture(
		device,
		deviceContext,
		"../DirectX11-Project/Textures/timer.tga",
		"../DirectX11-Project/Textures/timer.texturemeta",
		"timer"
	);

	return success;
}

bool TextureManager::LoadTexture(
	ID3D11Device* device,
	ID3D11DeviceContext* deviceContext,
	const char* texturePath,
	const char* textureMetaPath,
	const char* textureName
)
{
	// Create texture from targa image.
	TargaLoader targaLoader;
	Texture* texture = targaLoader.LoadTexture(texturePath, device, deviceContext);
	if (!texture) {
		return false;
	}
	// Load the sampler description from the texture meta data.
	TextureMetaLoader textureMetaLoader;
	D3D11_SAMPLER_DESC samplerDesc = textureMetaLoader.LoadSamplerSettings(textureMetaPath, device, deviceContext);
	// Create the texture sampler state if it doesn't exist.
	if (m_samplerBank.find(samplerDesc) == m_samplerBank.end()) {
		ID3D11SamplerState* samplerState;
		HRESULT result = device->CreateSamplerState(&samplerDesc, &samplerState);
		m_samplerBank[samplerDesc] = FAILED(result) ? 0 : samplerState;
	}
	// Set the sampler and register the texture.
	texture->SetSamplerState(m_samplerBank[samplerDesc]);
	m_textureBank[textureName] = texture;

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