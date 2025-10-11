#pragma once

/*======
INCLUDES
======*/
#include <d3d11.h>
#include <stdio.h>

#include "texture.h"

class TextureMetaLoader
{
public:
	TextureMetaLoader();
	TextureMetaLoader(const TextureMetaLoader& other);
	~TextureMetaLoader();

	bool LoadTextureMeta(Texture* texture, const char* filePath, ID3D11Device* device, ID3D11DeviceContext* deviceContext);

private:
	bool LoadSamplerSettings(Texture* texture, FILE* filePtr, ID3D11DeviceContext* deviceContext);
	void LoadSamplerFilter(D3D11_SAMPLER_DESC& samplerDesc, char* data);
	void LoadSamplerTextureAddressMode(D3D11_SAMPLER_DESC& samplerDesc, char* data);
	void LoadSamplerMipLODBias(D3D11_SAMPLER_DESC& samplerDesc, char* data);
	void LoadSamplerMaxAnisotropy(D3D11_SAMPLER_DESC& samplerDesc, char* data);
	void LoadSamplerComparisonFunction(D3D11_SAMPLER_DESC& samplerDesc, char* data);
	void LoadSamplerBorderColor(D3D11_SAMPLER_DESC& samplerDesc, char* data);
	void LoadSamplerMinimumLOD(D3D11_SAMPLER_DESC& samplerDesc, char* data);
	void LoadSamplerMaximumLOD(D3D11_SAMPLER_DESC& samplerDesc, char* data);

	static const char* s_delimiters;
};