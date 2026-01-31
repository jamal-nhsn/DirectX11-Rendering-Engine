#include "pch.h"
#include "texturemanager.h"
#include "src/Resources/resourceloaders.h"

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
	for (const std::filesystem::directory_entry& file : std::filesystem::directory_iterator("Resources/Textures")) {

		const std::filesystem::path& texturePath = file.path();
		
		// Ignore folders.
		if (!file.is_regular_file()) {
			continue;
		}
		
		// Create the path to the texturemeta file.
		std::filesystem::path textureMetaPath(texturePath.parent_path() / "Meta" / texturePath.stem());
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
	// Load the sampler description from the texture meta data.
	D3D11_SAMPLER_DESC samplerDesc = Engine::LoadTextureMeta(textureMetaPath);

	// Create the texture sampler state if it doesn't exist.
	if (m_samplerBank.find(samplerDesc) == m_samplerBank.end()) {
		ID3D11SamplerState* samplerState;
		HRESULT result = device->CreateSamplerState(&samplerDesc, &samplerState);
		m_samplerBank[samplerDesc] = FAILED(result) ? 0 : samplerState;
	}

	// Load the texture.
	std::unique_ptr<Engine::Texture2D> texture = Engine::LoadTGA(texturePath, device, deviceContext, m_samplerBank[samplerDesc]);

	if (!texture) {
		return false;
	}

	m_textureBank[texturePath.stem().string()] = std::move(texture);

	return true;
}


Engine::Texture2D* TextureManager::GetTexture(std::string textureName)
{
	return m_textureBank[textureName].get();
}

void TextureManager::Shutdown()
{
	for (auto& entry : m_samplerBank) {
		if (entry.second) {
			entry.second->Release();
		}
	}
	m_textureBank.clear();
}