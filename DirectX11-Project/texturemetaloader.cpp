#include "texturemetaloader.h"

const char* TextureMetaLoader::s_delimiters = ",() \n:\t";

TextureMetaLoader::TextureMetaLoader()
{
}

TextureMetaLoader::TextureMetaLoader(const TextureMetaLoader& other)
{
}

TextureMetaLoader::~TextureMetaLoader()
{
}

bool TextureMetaLoader::LoadTextureMeta(Texture* texture, const char* filePath, ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	int filePathLength = static_cast<int>(strlen(filePath));

	// Ensure the file path is long enough to contain ".texturemeta" at the end.
	if (filePathLength <= 12) {
		return 0;
	}

	char last5[12 + 1];
	strcpy_s(last5, &filePath[filePathLength - 12]);

	// Ensure the file extension is .texturemeta
	if (strcmp(last5, ".texturemeta") != 0) {
		return 0;
	}

	FILE* filePtr;
	int error = 0;

	// Open the file for reading.
	error = fopen_s(&filePtr, filePath, "r");
	if (error != 0) {
		return false;
	}

	char line[128];
	while (fgets(line, sizeof(line), filePtr) != 0) {
		if (strcmp(line, "#SAMPLING SETTINGS")) {
			LoadSamplerSettings(texture, filePtr, deviceContext);
		}
	}
}

bool TextureMetaLoader::LoadSamplerSettings(Texture* texture, FILE* filePtr, ID3D11DeviceContext* deviceContext)
{
	D3D11_SAMPLER_DESC samplerDesc;

	char line[128];
	while (fgets(line, sizeof(line), filePtr) != 0) {

		char* data = &line[0];
		char* field = strtok_s(data, s_delimiters, &data);
		if (!field) {
			continue;
		}

		if (strcmp(field, "Filter") == 0) {
			LoadSamplerFilter(samplerDesc, data);
		}
		else if (strcmp(field, "Texture_Address_Mode") == 0) {
			LoadSamplerTextureAddressMode(samplerDesc, data);
		}
		else if (strcmp(field, "Mip_LOD_Bias") == 0) {
			LoadSamplerMipLODBias(samplerDesc, data);
		}
		else if (strcmp(field, "Max_Anisotropy") == 0) {
			LoadSamplerMaxAnisotropy(samplerDesc, data);
		}
		else if (strcmp(field, "Comparison_Function") == 0) {
			LoadSamplerComparisonFunction(samplerDesc, data);
		}
		else if (strcmp(field, "Border_Color") == 0) {
			LoadSamplerBorderColor(samplerDesc, data);
		}
		else if (strcmp(field, "Minimum_LOD") == 0) {
			LoadSamplerMinimumLOD(samplerDesc, data);
		}
		else if (strcmp(field, "Maximum_LOD") == 0) {
			LoadSamplerMaximumLOD(samplerDesc, data);
		}
	}

	return true;
}

void TextureMetaLoader::LoadSamplerFilter(D3D11_SAMPLER_DESC& samplerDesc, char* data)
{
	char* token = strtok_s(data, s_delimiters, &data);
	// Default to bilinear filtering if input is malformed.
	if (token == 0) {
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
		return;
	}

	if (token && strcmp(token, "TRILINEAR") == 0) {
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		return;
	}
	if (token && strcmp(token, "BILINEAR") == 0) {
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
		return;
	}
	if (token && strcmp(token, "POINT") == 0) {
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		return;
	}

	unsigned int filter = 0;

	filter |= 0x10 * (token && strcmp(token, "LINEAR") == 0);

	token = strtok_s(data, s_delimiters, &data);
	filter |= 0x4 * (token && strcmp(token, "LINEAR") == 0);

	token = strtok_s(data, s_delimiters, &data);
	filter |= 0x1 * (token && strcmp(token, "LINEAR") == 0);

	samplerDesc.Filter = static_cast<D3D11_FILTER>(filter);
	return;
}

void TextureMetaLoader::LoadSamplerTextureAddressMode(D3D11_SAMPLER_DESC& samplerDesc, char* data)
{
	char* token = strtok_s(data, s_delimiters, &data);
	// Default to wrapped addressing if input is malformed.
	if (token == 0) {
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		return;
	}

	int addressModes[3] = { -1, -1, -1 };

	for (int i = 0; token && i < 3; i++) {
		if (token && strcmp(token, "WRAP") == 0) {
			addressModes[i] = static_cast<int>(D3D11_TEXTURE_ADDRESS_WRAP);
		}
		else if (token && strcmp(token, "CLAMP") == 0) {
			addressModes[i] = static_cast<int>(D3D11_TEXTURE_ADDRESS_CLAMP);
		}
		else if (token && strcmp(token, "MIRROR") == 0) {
			addressModes[i] = static_cast<int>(D3D11_TEXTURE_ADDRESS_MIRROR);
		}
		else if (token && strcmp(token, "MIRROR_ONCE") == 0) {
			addressModes[i] = static_cast<int>(D3D11_TEXTURE_ADDRESS_MIRROR_ONCE);
		}
		else if (token && strcmp(token, "BORDER") == 0) {
			addressModes[i] = static_cast<int>(D3D11_TEXTURE_ADDRESS_BORDER);
		}
		token = strtok_s(data, s_delimiters, &data);
	}

	// Again, default to wrapped addressing if input is malformed.
	samplerDesc.AddressU = addressModes[0] == -1 ? D3D11_TEXTURE_ADDRESS_WRAP : static_cast<D3D11_TEXTURE_ADDRESS_MODE>(addressModes[0]);
	samplerDesc.AddressV = addressModes[1] == -1 ? samplerDesc.AddressU : static_cast<D3D11_TEXTURE_ADDRESS_MODE>(addressModes[1]);
	samplerDesc.AddressW = addressModes[2] == -1 ? samplerDesc.AddressU : static_cast<D3D11_TEXTURE_ADDRESS_MODE>(addressModes[2]);
	return;
}

void TextureMetaLoader::LoadSamplerMipLODBias(D3D11_SAMPLER_DESC& samplerDesc, char* data)
{
}

void TextureMetaLoader::LoadSamplerMaxAnisotropy(D3D11_SAMPLER_DESC& samplerDesc, char* data)
{
}

void TextureMetaLoader::LoadSamplerComparisonFunction(D3D11_SAMPLER_DESC& samplerDesc, char* data)
{
}

void TextureMetaLoader::LoadSamplerBorderColor(D3D11_SAMPLER_DESC& samplerDesc, char* data)
{
}

void TextureMetaLoader::LoadSamplerMinimumLOD(D3D11_SAMPLER_DESC& samplerDesc, char* data)
{
}

void TextureMetaLoader::LoadSamplerMaximumLOD(D3D11_SAMPLER_DESC& samplerDesc, char* data)
{
}
