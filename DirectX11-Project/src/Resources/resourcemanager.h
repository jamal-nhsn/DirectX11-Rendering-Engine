#pragma once

#include "../Renderer/mesh.h"
#include "../Renderer/texture2d.h"

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

			static void LoadBuiltIn(std::unordered_map<std::string, std::unique_ptr<Mesh>>& meshBank);

			static std::unique_ptr<Mesh> LoadOBJ(const std::filesystem::path& filepath);
			static std::unique_ptr<Mesh> LoadBinary(const std::filesystem::path& filepath);
			static void CreateBinary(const std::filesystem::path& filepath, Mesh* mesh);
		};

		struct Texture2DLoader
		{
			struct BinaryHeader
			{
				unsigned int width;
				unsigned int height;
			};

			static void LoadBuiltIn(std::unordered_map<std::string, std::unique_ptr<Texture2D>>& texture2DBank, ID3D11SamplerState* sampler);

			static std::unique_ptr<Texture2D> LoadTGA(const std::filesystem::path& filepath, ID3D11SamplerState* sampler);
			static std::unique_ptr<Texture2D> LoadBinary(const std::filesystem::path& filepath, ID3D11SamplerState* sampler);
			static void CreateBinary(const std::filesystem::path& filepath, Texture2D* texture2D);
		};

	public:
		ResourceManager(bool useBinaries);
		
		ResourceManager(const ResourceManager& other) = delete;
		ResourceManager& operator=(const ResourceManager& other) = delete;

		ResourceManager(ResourceManager&& other) = default;
		ResourceManager& operator=(ResourceManager&& other) = default;

		~ResourceManager() = default;

	public:
		void LoadMesh(const std::filesystem::path& filepath); // Loads mesh at filepath.
		void LoadMesh(const std::string& filepath);           // Looks in s_meshDirPath if no parent path is provided.
		void LoadAllMeshes();                                 // Loads all in s_meshDirPath.

		Mesh* GetMesh(std::string meshName);

		void LoadTexture2D(const std::filesystem::path& filepath);
		void LoadTexture2D(const std::string& filepath);
		void LoadAllTexture2Ds();

		Texture2D* GetTexture2D(std::string texture2DName);

		/*
		void LoadSpriteAnimation(const std::filesystem::path& filepath);
		void LoadSpriteAnimation(const std::string& filepath);
		void LoadAllSpriteAnimations();
		*/

	private:
		std::unordered_map<std::string, std::unique_ptr<Mesh>> m_meshBank;
		std::unordered_map<D3D11_SAMPLER_DESC, ID3D11SamplerState*> m_samplerBank;
		std::unordered_map<std::string, std::unique_ptr<Texture2D>> m_texture2DBank;

		bool m_useBinaries;
	};
}