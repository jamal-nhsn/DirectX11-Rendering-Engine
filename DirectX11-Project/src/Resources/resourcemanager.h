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
		struct SamplerDescEqual
		{
			bool operator()(const D3D11_SAMPLER_DESC& a, const D3D11_SAMPLER_DESC& b) const noexcept
			{
				return a.Filter == b.Filter &&
					a.AddressU == b.AddressU &&
					a.AddressV == b.AddressV &&
					a.AddressW == b.AddressW &&
					a.MipLODBias == b.MipLODBias &&
					a.MaxAnisotropy == b.MaxAnisotropy &&
					a.ComparisonFunc == b.ComparisonFunc &&
					a.MinLOD == b.MinLOD &&
					a.MaxLOD == b.MaxLOD &&
					a.BorderColor[0] == b.BorderColor[0] &&
					a.BorderColor[1] == b.BorderColor[1] &&
					a.BorderColor[2] == b.BorderColor[2] &&
					a.BorderColor[3] == b.BorderColor[3];
			}
		};

		struct SamplerDescHash
		{
			size_t operator()(const D3D11_SAMPLER_DESC& desc) const noexcept
			{
				auto hash_combine = [](size_t& seed, auto v)
					{
						std::hash<std::decay_t<decltype(v)>> h;
						seed ^= h(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
					};

				size_t h = 0;
				hash_combine(h, desc.Filter);
				hash_combine(h, desc.AddressU);
				hash_combine(h, desc.AddressV);
				hash_combine(h, desc.AddressW);
				hash_combine(h, desc.MipLODBias);
				hash_combine(h, desc.MaxAnisotropy);
				hash_combine(h, desc.ComparisonFunc);
				hash_combine(h, desc.MinLOD);
				hash_combine(h, desc.MaxLOD);
				hash_combine(h, desc.BorderColor[0]);
				hash_combine(h, desc.BorderColor[1]);
				hash_combine(h, desc.BorderColor[2]);
				hash_combine(h, desc.BorderColor[3]);
				return h;
			}
		};

	private:
		std::unordered_map<std::string, std::unique_ptr<Mesh>> m_meshBank;
		std::unordered_map<D3D11_SAMPLER_DESC, ID3D11SamplerState*, SamplerDescHash, SamplerDescEqual> m_samplerBank;
		std::unordered_map<std::string, std::unique_ptr<Texture2D>> m_texture2DBank;

		bool m_useBinaries;
	};
}