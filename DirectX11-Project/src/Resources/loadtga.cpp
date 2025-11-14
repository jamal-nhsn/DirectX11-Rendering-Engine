#include "resourceloaders.h"

#include <fstream>
#include <vector>
#include <cassert>

namespace Engine
{
	std::shared_ptr<Texture2D> LoadTGA(const std::filesystem::path& filepath, ID3D11Device* device, ID3D11DeviceContext* deviceContext, ID3D11SamplerState* samplerState)
	{
		// Invalid filepath.
		assert(("Error: Image passed to LoadTGA is not a .tga file!", filepath.extension() == ".tga"));

		// Open the file.
		std::ifstream file(filepath, std::ios::binary);
		if (!file.is_open()) {
			return 0;
		}

		struct Header
		{
			unsigned char  data1[12];
			unsigned short width;
			unsigned short height;
			unsigned char  bpp;
			unsigned char  imageDescriptor;
		};

		// Read in the header data.
		Header fileHeader;
		file.read(reinterpret_cast<char*>(&fileHeader), sizeof(Header));

		unsigned int width = static_cast<unsigned int>(fileHeader.width);
		unsigned int height = static_cast<unsigned int>(fileHeader.height);
		unsigned int bpp = static_cast<unsigned int>(fileHeader.bpp);

		// Ensure that the image data is 32 bits per pixel and not 24.
		if (bpp != 32) {
			return 0;
		}

		// Calculate the size of the image data (in bytes).
		unsigned int imageSize = width * height * 4;

		// Allocate space for the image data.
		std::vector<char> imageData(imageSize, 0);

		// Read in the image data.
		file.read(imageData.data(), imageSize);

		// Check to see if the image needs to be flipped.
		bool flipImage = fileHeader.imageDescriptor & (0b00100000);

		// Initialize the index into the image data array.
		unsigned int dataIndex = flipImage * ((width * height * 4) - (width * 4));

		// Initialize the index into the image destination data array.
		unsigned int destIndex = 0;

		// Allocate space for the cleaned up image data.
		std::vector<char> destData(imageSize, 0);

		// Now, copy the image data into the destination array in the correct order, since the targa format is not in RGBA order and may be upside down.
		for (unsigned int j = 0; j < height; j++) {
			for (unsigned int i = 0; i < width; i++) {

				destData[destIndex + 0] = imageData[dataIndex + 2]; // Red.
				destData[destIndex + 1] = imageData[dataIndex + 1]; // Green.
				destData[destIndex + 2] = imageData[dataIndex + 0]; // Blue.
				destData[destIndex + 3] = imageData[dataIndex + 3]; // Alpha.

				// Increment the indices into the data.
				dataIndex += 4;
				destIndex += 4;
			}
			// Set the image data index back to the preceding row at the beginning of the column since its reading it in upside down
			dataIndex += -(flipImage * static_cast<int>(width) * 8);
		}

		// Create the texture.
		int rowPitch = width * 4;
		return std::make_shared<Texture2D>(device, deviceContext, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, destData.data(), rowPitch, samplerState);
	}
}