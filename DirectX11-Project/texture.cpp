#include "texture.h"

Texture::Texture()
{
	m_targaData   = 0;
	m_texture     = 0;
	m_textureView = 0;

	m_width  = 0;
	m_height = 0;
}

Texture::Texture(const Texture& other)
{
	m_targaData   = other.m_targaData;
	m_texture     = other.m_texture;
	m_textureView = other.m_textureView;

	m_width  = other.m_width;
	m_height = other.m_height;
}

Texture::~Texture()
{
}

bool Texture::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const char* fileName)
{
	bool result;
	D3D11_TEXTURE2D_DESC textureDesc;
	HRESULT hResult;
	unsigned int rowPitch;
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;

	// Load the targa image data into memory
	result = LoadTarga32Bit(fileName);
	if (!result) {
		return result;
	}

	// Setup texture description
	textureDesc.Width = m_width;
	textureDesc.Height = m_height;
	textureDesc.MipLevels = 0;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

	// Create the empty texture
	hResult = device->CreateTexture2D(&textureDesc, NULL, &m_texture);
	if (FAILED(hResult)) {
		return false;
	}

	// Set the row pitch of the targa image data
	rowPitch = (m_width * 4) * sizeof(unsigned char);

	// Copy the targa image data into the texture
	deviceContext->UpdateSubresource(m_texture, 0, NULL, m_targaData, rowPitch, 0);

	// Setup the shader resource view description
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = -1;

	// Create the shader resource view for the texture
	hResult = device->CreateShaderResourceView(m_texture, &srvDesc, &m_textureView);
	if (FAILED(hResult)) {
		return false;
	}

	// Generate mipmaps for this texture
	deviceContext->GenerateMips(m_textureView);

	// Release the targa image data now that the image data has been loaded into the texture
	delete[] m_targaData;
	m_targaData = 0;

	return true;
}

void Texture::Shutdown()
{
	// Release the texture view resource
	if (m_textureView) {
		m_textureView->Release();
		m_textureView = 0;
	}

	// Release the texture
	if (m_texture) {
		m_texture->Release();
		m_texture = 0;
	}

	// Release the targa data
	if (m_targaData) {
		delete[] m_targaData;
		m_targaData = 0;
	}

	return;
}

ID3D11ShaderResourceView* Texture::GetTexture2D()
{
	return m_textureView;
}

int Texture::GetWidth()
{
	return m_width;
}

int Texture::GetHeight()
{
	return m_height;
}

bool Texture::LoadTarga32Bit(const char* fileName)
{
	int error;
	int bpp;
	int imageSize;
	int index;
	int i;
	int j;
	int k;
	FILE* filePtr;
	unsigned int count;
	TargaHeader targaFileHeader;
	unsigned char* targaImage;

	// Open the targa file for reading in binary
	error = fopen_s(&filePtr, fileName, "rb");
	if (error != 0) {
		return false;
	}

	// Read in the file header
	count = (unsigned int)fread(&targaFileHeader, sizeof(TargaHeader), 1, filePtr);
	if (count != 1) {
		return false;
	}

	// Get the important information from the header
	m_width = (int)targaFileHeader.width;
	m_height = (int)targaFileHeader.height;
	bpp = (int)targaFileHeader.bpp;

	// Ensure that it is 32 bit and 24 bit
	if (bpp != 32) {
		return false;
	}

	// Calculate the size of the 32 bit image data (in bytes)
	imageSize = m_width * m_height * 4;

	// Allocate memory for the targa image data
	targaImage = new unsigned char[imageSize];

	// Read in the targa image data
	count = (unsigned int)fread(targaImage, 1, imageSize, filePtr);
	if (count != imageSize) {
		return false;
	}

	// Close the file
	error = fclose(filePtr);
	if (error != 0) {
		return false;
	}
	// Allocate memory for the targa destination data
	m_targaData = new unsigned char[imageSize];

	// Initialize the index into the targa destination data array.
	index = 0;

	// Initialize the index into the targa image data
	k = (m_width * m_height * 4) - (m_width * 4);

	// Now, copy the targa image data into the targa destination array in the correct order since the targa format is stored upside down and also is not in RGBA order
	for (j = 0; j < m_height; j++) {
		for (i = 0; i < m_width; i++) {
			m_targaData[index + 0] = targaImage[k + 2]; // Red
			m_targaData[index + 1] = targaImage[k + 1]; // Green
			m_targaData[index + 2] = targaImage[k + 0]; // Blue
			m_targaData[index + 3] = targaImage[k + 3]; // Alpha

			// Increment the indexes into the targa data.
			k += 4;
			index += 4;
		}
		// Set the targa image data index back to the preceding row at the beginning of the column since its reading it in upside down
		k -= (m_width * 8);
	}

	// Release the targa image daa now that it was copied into the destination array
	delete[] targaImage;
	targaImage = 0;

	return true;
}

