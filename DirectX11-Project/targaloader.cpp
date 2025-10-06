#include "targaloader.h"

TargaLoader::TargaLoader()
{
}

TargaLoader::TargaLoader(const TargaLoader& other)
{
}

TargaLoader::~TargaLoader()
{
}

Texture* TargaLoader::LoadTexture(const char* filePath, ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	// Open the targa file for reading in binary.
	FILE* filePtr;
	int error = fopen_s(&filePtr, filePath, "rb");
	if (error != 0) {
		return 0;
	}

	// Read in the file header.
	Header fileHeader;
	int count = (unsigned int)fread(&fileHeader, sizeof(Header), 1, filePtr);
	if (count != 1) {
		fclose(filePtr);
		return 0;
	}

	// Get the important information from the header.
	int width  = (int)fileHeader.width;
	int height = (int)fileHeader.height;
	int bpp    = (int)fileHeader.bpp;

	// Ensure that the image data is in 32 bits and not 24 bits.
	if (bpp != 32) {
		fclose(filePtr);
		return 0;
	}

	// Calculate the size of the 32 bit image data(in bytes).
	int imageSize = width * height * 4;

	// Allocate memory for the image.
	unsigned char* imageData = new unsigned char[imageSize];

	// Read in the image data
	count = (unsigned int)fread(imageData, 1, imageSize, filePtr);
	if (count != imageSize) {
		fclose(filePtr);
		delete[] imageData;
		return 0;
	}

	// Close the file
	error = fclose(filePtr);
	if (error != 0) {
		delete[] imageData;
		return 0;
	}

	// Allocate memory for the image destination data.
	unsigned char* destData = new unsigned char[imageSize];

	// Initialize the index into the image data array.
	int dataIndex = (width * height * 4) - (width * 4);

	// Initialize the index into the image destination data array.
	int destIndex = 0;
	
	// Now, copy the image data into the destination array in the correct order, since the targa format is stored uspide down and alos is not in RGBA order.
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {

			destData[destIndex + 0] = imageData[dataIndex + 2]; // Red.
			destData[destIndex + 1] = imageData[dataIndex + 1]; // Green.
			destData[destIndex + 2] = imageData[dataIndex + 0]; // Blue.
			destData[destIndex + 3] = imageData[dataIndex + 3]; // Alpha.

			// Increment the indices into the data.
			dataIndex += 4;
			destIndex += 4;
		}
		// Set the image data index back to the preceding row at the beginning of the column since its reading it in upside down
		dataIndex -= (width * 8);
	}

	// Release the image data now that it was copied into the destination array.
	delete[] imageData;
	
	// Create the texture and texture view.
	ID3D11Texture2D*          texture;
	ID3D11ShaderResourceView* textureView;

	// Setup texture description.
	D3D11_TEXTURE2D_DESC textureDesc;
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 0;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

	// Create the empty texture.
	HRESULT result = device->CreateTexture2D(&textureDesc, 0, &texture);
	if (FAILED(result)) {
		delete[] destData;
		return 0;
	}

	// Set the row pitch of the image data.
	int rowPitch = (width * 4) * sizeof(unsigned char);

	// Copy the image data into the texture.
	deviceContext->UpdateSubresource(texture, 0, 0, destData, rowPitch, 0);

	// Setup the shader resource view description.
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = -1;

	// Create the shader resource view for the shader.
	result = device->CreateShaderResourceView(texture, &srvDesc, &textureView);
	if (FAILED(result)) {
		return 0;
	}

	// Generate mipmaps for the texture
	deviceContext->GenerateMips(textureView);

	// Release the image data now that it has been loaded into a texture.
	delete[] destData;

	// Create the Texture object itself.
	Texture* textureObject = new Texture;

	// Initialize the Texture object.
	bool success = textureObject->Initialize(texture, textureView, width, height);
	if (!success) {
		delete textureObject;
		return 0;
	}
	return textureObject;
}