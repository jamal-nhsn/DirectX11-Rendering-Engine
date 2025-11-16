#pragma once

#include "../Renderer/mesh.h"

#include <string>
#include <filesystem>
#include <memory>
#include <unordered_map>

namespace Engine
{
	class ResourceManager
	{
	private:
		static constexpr const char* s_resourceDirPath = "Resources";
		
		static constexpr const char* s_meshDirPath            = "Resources/Meshes";
		static constexpr const char* s_spriteAnimationDirPath = "Resources/SpriteAnimations";
		static constexpr const char* s_textureDirPath         = "Resources/Textures";
		static constexpr const char* s_textureMetaDirPath     = "Resources/Textures/Meta";

		static constexpr const char* s_meshBinaryPath            = "Resources/Meshes/.bin";
		static constexpr const char* s_spriteAnimationBinaryPath = "Resources/SpriteAnimations/.bin";
		static constexpr const char* s_textureBinaryPath         = "Resources/Textures/.bin";
		static constexpr const char* s_textureMetaBinaryPath     = "Resources/Textures/Meta/.bin";

	private:
		struct MeshLoader
		{
			struct BinaryHeader
			{
				unsigned int vertexCount;
				unsigned int indexCount;
			};

			static void LoadBuiltIn(std::unordered_map<std::string, std::unique_ptr<Mesh>>& meshBank, ID3D11Device* device);

			static std::unique_ptr<Mesh> LoadMeshOBJ(const std::filesystem::path& filepath, ID3D11Device* device);
			static std::unique_ptr<Mesh> LoadMeshBinary(const std::filesystem::path& filepath, ID3D11Device* device);
			static void CreateBinary(const std::filesystem::path& filepath, Mesh* mesh);
		};

	public:
		ResourceManager(bool useBinaries);
		
		ResourceManager(const ResourceManager& other) = delete;
		ResourceManager& operator=(const ResourceManager& other) = delete;

		ResourceManager(ResourceManager&& other) = default;
		ResourceManager& operator=(ResourceManager&& other) = default;

		~ResourceManager() = default;

	public:
		bool LoadMesh(const std::string& fileName);

		Mesh* GetMesh(std::string meshName);

		/*
		bool LoadTexture2D(const std::filesystem::path& filepath);
		bool LoadTexture2D(const std::string& filepath);
		bool LoadTexture2D(const char* filepath);

		bool LoadSpriteAnimation(const std::filesystem::path& filepath);
		bool LoadSpriteAnimation(const std::string& filepath);
		bool LoadSpriteAnimation(const char* filepath);
		*/

	private:
		std::unordered_map<std::string, std::unique_ptr<Mesh>> m_meshBank;

		bool m_useBinaries;
	};
}