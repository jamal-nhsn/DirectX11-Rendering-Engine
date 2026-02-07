#include "pch.h"
#include "resourcemanager.h"

namespace Engine
{
	void ResourceManager::Texture2DLoader::LoadBuiltIn(std::unordered_map<std::string, std::unique_ptr<Texture2D>>& texture2DBank, Sampler* sampler)
	{
		// Solid-color.
		char white[] = { 255, 255, 255, 255 };
		char red[]   = { 255,   0,   0, 255 };
		char green[] = { 0,   255,   0, 255 };
		char blue[]  = { 0,     0, 255, 255 };
		char black[] = { 0,     0,   0, 255 };

		// Meta.
		char error[] = {
			255,   0, 255, 255,
			  0,   0, 0,   255,
			  0,   0, 0,   255,
			255,   0, 255, 255
		};
		char defaultNormal[] = { 128, 128, 255, 255 };

		texture2DBank["white"] = std::make_unique<Texture2D>(white, 1, 1, sampler);
		texture2DBank["red"]   = std::make_unique<Texture2D>(red, 1, 1, sampler);
		texture2DBank["green"] = std::make_unique<Texture2D>(green, 1, 1, sampler);
		texture2DBank["blue"]  = std::make_unique<Texture2D>(blue, 1, 1, sampler);
		texture2DBank["black"] = std::make_unique<Texture2D>(black, 1, 1, sampler);

		texture2DBank["error"]         = std::make_unique<Texture2D>(error, 2, 2, sampler);
		texture2DBank["defaultNormal"] = std::make_unique<Texture2D>(defaultNormal, 1, 1, sampler);
	}

	std::unique_ptr<Texture2D> ResourceManager::Texture2DLoader::LoadTGA(const std::filesystem::path& filepath, Sampler* sampler)
	{
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
		return std::make_unique<Texture2D>(destData.data(), width, height, sampler);
	}

	std::unique_ptr<Texture2D> ResourceManager::Texture2DLoader::LoadBinary(const std::filesystem::path& filepath, Sampler* sampler)
	{
		std::ifstream file(filepath, std::ios::binary);
		
		if (!file.is_open()) {
			return 0;
		}

		BinaryHeader header;
		file.read(reinterpret_cast<char*>(&header), sizeof(BinaryHeader));

		std::vector<char> pixels(4 * header.width * header.height);
		file.read(reinterpret_cast<char*>(pixels.data()), pixels.size());

		return std::make_unique<Texture2D>(pixels.data(), header.width, header.height, sampler);
	}

	void ResourceManager::Texture2DLoader::CreateBinary(const std::filesystem::path& filepath, Texture2D* texture2D)
	{
		assert(("Error: Texture2D cannot be null!", texture2D));

		std::ofstream file(filepath, std::ios::binary);
		assert(("Error: Cannot create texture2D binary file!", file.is_open()));

		// Create the header.
		BinaryHeader header{
			texture2D->GetWidth(),
			texture2D->GetHeight()
		};

		// Write the header.
		file.write(reinterpret_cast<const char*>(&header), sizeof(BinaryHeader));

		const std::vector<char>& pixelData = texture2D->GetData();
		file.write(pixelData.data(), pixelData.size());
	}
}