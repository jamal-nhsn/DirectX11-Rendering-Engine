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
	bool success = false;
	for (const auto& file : std::filesystem::directory_iterator("../DirectX11-Project/Textures/")) {
		// Get the path to the texture itself.
		const auto& texturePath = file.path();

		// Ignore all texturemeta files.
		if (texturePath.extension().compare(".texturemeta") == 0) {
			continue;
		}
		
		// Create the path to the texturemeta file.
		std::filesystem::path textureMetaPath(texturePath);
		textureMetaPath.replace_extension(".texturemeta");
	
		// Load the texture.
		success = LoadTexture(device, deviceContext, texturePath, textureMetaPath);

		if (!success) {
			return success;
		}
	}

	return success;
}

bool TextureManager::LoadTexture(
	ID3D11Device* device,
	ID3D11DeviceContext* deviceContext,
	const std::filesystem::path& texturePath,
	const std::filesystem::path& textureMetaPath
)
{
	Texture* texture = 0;
	if (texturePath.extension().compare(".tga") == 0) {
		// Create texture from targa image.
		TargaLoader targaLoader;
		texture = targaLoader.LoadTexture(texturePath.string().c_str(), device, deviceContext);
	}
	else
	{
		// not a tga file, ignore it.
		return true;
	}
	if (!texture) {
		return false;
	}

	// Load the sampler description from the texture meta data.
	TextureMetaLoader textureMetaLoader;
	D3D11_SAMPLER_DESC samplerDesc = textureMetaLoader.LoadSamplerSettings(textureMetaPath.string().c_str(), device, deviceContext);
	// Create the texture sampler state if it doesn't exist.
	if (m_samplerBank.find(samplerDesc) == m_samplerBank.end()) {
		ID3D11SamplerState* samplerState;
		HRESULT result = device->CreateSamplerState(&samplerDesc, &samplerState);
		m_samplerBank[samplerDesc] = FAILED(result) ? 0 : samplerState;
	}
	// Set the sampler and register the texture.
	texture->SetSamplerState(m_samplerBank[samplerDesc]);
	m_textureBank[texturePath.stem().string()] = texture;

	return true;
}


Texture* TextureManager::GetTexture(std::string textureName)
{
	return m_textureBank[textureName];
}

void TextureManager::Shutdown()
{
	for (auto& entry : m_textureBank) {
		if (entry.second) {
			entry.second->Shutdown();
		}
		delete entry.second;
	}
	for (auto& entry : m_samplerBank) {
		if (entry.second) {
			entry.second->Release();
		}
	}
	m_textureBank.clear();
}