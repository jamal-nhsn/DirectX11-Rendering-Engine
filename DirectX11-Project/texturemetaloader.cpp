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

	bool success = true;
	char line[128];
	while (success && fgets(line, sizeof(line), filePtr) != 0) {
		if (strcmp(line, "#SAMPLING SETTINGS")) {
			success = success && LoadSamplerSettings(texture, filePtr, device, deviceContext);
		}
	}

	// Close the file
	fclose(filePtr);

	return success;
}

bool TextureMetaLoader::LoadSamplerSettings(Texture* texture, FILE* filePtr, ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	D3D11_SAMPLER_DESC samplerDesc;
	// Initialize default sampler description in case input is malformed.
	samplerDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

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

	// Create the texture sampler state.
	ID3D11SamplerState* samplerState;
	HRESULT result = device->CreateSamplerState(&samplerDesc, &samplerState);
	if (!FAILED(result)) {
		texture->SetSamplerState(samplerState);
	}

	return !FAILED(result);
}

void TextureMetaLoader::LoadSamplerFilter(D3D11_SAMPLER_DESC& samplerDesc, char* data)
{
	char* token = strtok_s(data, s_delimiters, &data);
	// Use defualt value if input is malformed.
	if (token == 0) {
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
	// Use defualt values if input is malformed.
	if (token == 0) {
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
	char* token = strtok_s(data, s_delimiters, &data);
	// Use defualt value if input is malformed.
	if (token == 0) {
		return;
	}

	samplerDesc.MipLODBias = strtof(token, &token);
	return;
}

void TextureMetaLoader::LoadSamplerMaxAnisotropy(D3D11_SAMPLER_DESC& samplerDesc, char* data)
{
	char* token = strtok_s(data, s_delimiters, &data);
	// Use defualt value if input is malformed.
	if (token == 0) {
		return;
	}

	int value = strtol(token, &token, 10);
	samplerDesc.MaxAnisotropy = value < 1 ? 1 : value;
	return;
}

void TextureMetaLoader::LoadSamplerComparisonFunction(D3D11_SAMPLER_DESC& samplerDesc, char* data)
{
	char* token = strtok_s(data, s_delimiters, &data);
	// Use defualt value if input is malformed.
	if (token == 0) {
		return;
	}

	if (strcmp(token, "NEVER") == 0) {
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	}
	else if (strcmp(token, "LESS") == 0) {
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_LESS;
	}
	else if (strcmp(token, "EQUAL") == 0) {
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_EQUAL;
	}
	else if (strcmp(token, "LESS_EQUAL") == 0) {
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
	}
	else if (strcmp(token, "GREATER") == 0) {
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_GREATER;
	}
	else if (strcmp(token, "NOT_EQUAL") == 0) {
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_NOT_EQUAL;
	}
	else if (strcmp(token, "GREATER_EQUAL") == 0) {
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_GREATER_EQUAL;
	}
	else if (strcmp(token, "ALWAYS") == 0) {
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	}

	return;
}

void TextureMetaLoader::LoadSamplerBorderColor(D3D11_SAMPLER_DESC& samplerDesc, char* data)
{
	char* token = strtok_s(data, s_delimiters, &data);
	// Use defualt value if input is malformed.
	if (token == 0) {
		return;
	}

	float divisor = strcmp(token, "I") == 0 ? 255.0f : 1.0f;

	for (int i = 0; i < 4; i++) {
		token = strtok_s(data, s_delimiters, &data);
		if (token != 0) {
			float value = static_cast<float>(strtof(token, &token)) / divisor;
			samplerDesc.BorderColor[i] = value < 0.0f ? 0.0f : value > 1.0f ? 1.0f : value;
		}
	}

	return;
}

void TextureMetaLoader::LoadSamplerMinimumLOD(D3D11_SAMPLER_DESC& samplerDesc, char* data)
{
	char* token = strtok_s(data, s_delimiters, &data);
	// Use defualt value if input is malformed.
	if (token == 0) {
		return;
	}

	float value = strtof(token, &token);
	samplerDesc.MinLOD = value < 0.0f ? 0.0f : value;
	return;
}

void TextureMetaLoader::LoadSamplerMaximumLOD(D3D11_SAMPLER_DESC& samplerDesc, char* data)
{
	char* token = strtok_s(data, s_delimiters, &data);
	// Use defualt value if input is malformed.
	if (token == 0) {
		return;
	}

	if (strcmp(token, "MAX") == 0) {
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
		return;
	}

	float value = strtof(token, &token);
	samplerDesc.MaxLOD = value < samplerDesc.MinLOD ? samplerDesc.MinLOD : value;
	return;
}
