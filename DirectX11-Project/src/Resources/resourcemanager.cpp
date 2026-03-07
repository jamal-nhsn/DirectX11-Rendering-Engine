#include "pch.h"
#include "resourcemanager.h"

namespace Engine
{
	ResourceManager::ResourceManager(bool useBinaries)
		: m_useBinaries(useBinaries)
	{
		MeshLoader::LoadBuiltIn(m_meshBank);
		SamplerLoader::LoadBuiltIn(m_samplerBank);
		Texture2DLoader::LoadBuiltIn(m_texture2DBank, m_samplerBank[SamplerLoader::s_defaultSamplerDesc].get());

		// Create the file hierarchy.
		std::filesystem::create_directory(s_resourceDirPath);

		std::filesystem::create_directory(s_meshDirPath);
		std::filesystem::create_directory(s_spriteAnimationDirPath);
		std::filesystem::create_directory(s_textureDirPath);
		std::filesystem::create_directory(s_samplerDirPath);

		std::filesystem::create_directory(s_meshBinaryDirPath);
		std::filesystem::create_directory(s_textureBinaryDirPath);
		std::filesystem::create_directory(s_samplerBinaryDirPath);
	}

	void ResourceManager::LoadMesh(const std::filesystem::path& filepath)
	{
		assert(("Error: Mesh does not exist!", std::filesystem::exists(filepath)));

		const std::string& meshName = filepath.stem().string();
		assert(("Error: Mesh name is already taken!", m_meshBank.find(meshName) == m_meshBank.end()));

		// Create path to binary file.
		std::filesystem::path binaryPath(s_meshBinaryDirPath);
		binaryPath /= meshName + ".bin";

		if (m_useBinaries) {
			// Only use binary file if it is newer.
			bool shouldUseBinary =
				std::filesystem::exists(binaryPath) &&
				std::filesystem::last_write_time(binaryPath) > std::filesystem::last_write_time(filepath);

			if (shouldUseBinary) {
				m_meshBank[meshName] = MeshLoader::LoadBinary(binaryPath);
				assert(("Error: Could not load Mesh!", m_meshBank[meshName]));
				return;
			}
		}

		const std::filesystem::path& extension = filepath.extension();

		if (extension == ".meshobj") {
			m_meshBank[meshName] = MeshLoader::LoadOBJ(filepath);
			assert(("Error: Could not load Mesh!", m_meshBank[meshName]));
		}
		else {
			assert(("Error: Unsupported Mesh type!", false));
		}

		// Generate binary file.
		if (m_useBinaries) {
			MeshLoader::CreateBinary(binaryPath, m_meshBank[meshName].get());
		}
	}

	void ResourceManager::LoadMesh(const std::string& filepath)
	{
		std::filesystem::path path(filepath);
		if (path.has_root_directory()) {
			LoadMesh(path);
		}
		else {
			std::filesystem::path truePath(s_meshDirPath / path);
			LoadMesh(truePath);
		}
	}

	void ResourceManager::LoadAllMeshes()
	{
		std::filesystem::path binaryPath(s_meshBinaryDirPath);

		for (
			auto it = std::filesystem::recursive_directory_iterator(s_meshDirPath);
			it != std::filesystem::recursive_directory_iterator();
			it++
			) {
			// Ignore the binary directory.
			if (it->is_directory() && it->path().lexically_normal() == binaryPath.lexically_normal()) {
				it.disable_recursion_pending();
				continue;
			}

			if (it->is_regular_file()) {
				LoadMesh(it->path());
			}
		}
	}

	Mesh* ResourceManager::GetMesh(std::string meshName)
	{
		assert(("Error: Mesh does not exist!", m_meshBank.find(meshName) != m_meshBank.end()));
		assert(("Error: Mesh is null!", m_meshBank[meshName]));

		return m_meshBank[meshName].get();
	}

	void ResourceManager::LoadTexture2D(const std::filesystem::path& filepath)
	{
		assert(("Error: Texture does not exist!", std::filesystem::exists(filepath)));

		const std::string& texture2DName = filepath.stem().string();
		assert(("Error: Texture2D name is already taken!", m_texture2DBank.find(texture2DName) == m_texture2DBank.end()));

		// Create path to sampler file.
		std::filesystem::path samplerPath(s_samplerDirPath);
		samplerPath /= texture2DName + ".sampler";

		// Create .sampler file if it doesn't exist.
		if (!std::filesystem::exists(samplerPath)) {
			SamplerLoader::CreateSampler(samplerPath);
		}

		// Create path to sampler binary file.
		std::filesystem::path samplerBinary(s_samplerBinaryDirPath);
		samplerBinary /= texture2DName + ".bin";

		// Load sampler.

		std::unique_ptr<Sampler> sampler;
		Sampler* samplerRaw;
		D3D11_SAMPLER_DESC samplerDesc;

		bool shouldUseBinary =
			m_useBinaries &&
			std::filesystem::exists(samplerBinary) &&
			std::filesystem::last_write_time(samplerBinary) > std::filesystem::last_write_time(samplerPath);

		if (shouldUseBinary) {
			sampler = SamplerLoader::LoadBinary(samplerBinary);
			assert(("Error: Could not load Sampler!", sampler));
		}
		else {
			const std::filesystem::path& extension = samplerPath.extension();

			if (extension == ".sampler") {
				sampler = SamplerLoader::LoadSampler(samplerPath);
				assert(("Error: Could not load Sampler!", sampler));
			}
			else {
				assert(("Error: Unsupported Sampler type!", false));
			}
		}

		samplerDesc = sampler->GetDesc();

		if (m_samplerBank.find(samplerDesc) == m_samplerBank.end()) {
			m_samplerBank[samplerDesc] = std::move(sampler);
		}

		samplerRaw = m_samplerBank[samplerDesc].get();

		// Create path to the texture binary file.
		std::filesystem::path texture2DBinary(s_textureBinaryDirPath);
		texture2DBinary /= texture2DName + ".bin";

		// Load texture.

		shouldUseBinary =
			m_useBinaries &&
			std::filesystem::exists(texture2DBinary) &&
			std::filesystem::last_write_time(texture2DBinary) > std::filesystem::last_write_time(filepath);

		if (shouldUseBinary) {
			m_texture2DBank[texture2DName] = Texture2DLoader::LoadBinary(texture2DBinary, samplerRaw);
			assert(("Error: Could not load Texture2D!", m_texture2DBank[texture2DName]));
		}
		else {
			const std::filesystem::path& extension = filepath.extension();

			if (extension == ".tga") {
				m_texture2DBank[texture2DName] = Texture2DLoader::LoadTGA(filepath, samplerRaw);
				assert(("Error: Could not load Texture2D!", m_texture2DBank[texture2DName]));
			}
			else {
				assert(("Error: Unsupported Texture2D type!", false));
			}
		}

		// Create binary files.
		if (m_useBinaries) {
			if (!std::filesystem::exists(samplerBinary) || std::filesystem::last_write_time(samplerBinary) < std::filesystem::last_write_time(samplerPath)) {
				SamplerLoader::CreateBinary(samplerBinary, &samplerDesc);
			}
			if (!std::filesystem::exists(texture2DBinary) || std::filesystem::last_write_time(texture2DBinary) < std::filesystem::last_write_time(filepath)) {
				Texture2DLoader::CreateBinary(texture2DBinary, m_texture2DBank[texture2DName].get());
			}
		}
	}

	void ResourceManager::LoadTexture2D(const std::string& filepath)
	{
		std::filesystem::path path(filepath);
		if (path.has_root_directory()) {
			LoadTexture2D(path);
		}
		else {
			std::filesystem::path truePath(s_textureDirPath / path);
			LoadTexture2D(truePath);
		}
	}

	void ResourceManager::LoadAllTexture2Ds()
	{
		std::filesystem::path binaryPath(s_textureBinaryDirPath);
		std::filesystem::path samplerPath(s_samplerDirPath);

		for (
			auto it = std::filesystem::recursive_directory_iterator(s_textureDirPath);
			it != std::filesystem::recursive_directory_iterator();
			it++
			) {
			// Ignore the binary and sampler directories.
			if (it->is_directory() && it->path().lexically_normal() == binaryPath.lexically_normal() ||
				it->is_directory() && it->path().lexically_normal() == samplerPath.lexically_normal()) {
				it.disable_recursion_pending();
				continue;
			}

			if (it->is_regular_file()) {
				LoadTexture2D(it->path());
			}
		}
	}

	Texture2D* ResourceManager::GetTexture2D(std::string texture2DName)
	{
		assert(("Error: Texture2D does not exist!", m_texture2DBank.find(texture2DName) != m_texture2DBank.end()));
		assert(("Error: Texture2D is null!", m_texture2DBank[texture2DName]));

		return m_texture2DBank[texture2DName].get();
	}

	void ResourceManager::LoadSpriteAnimation(const std::filesystem::path& filepath)
	{
		assert(("Error: SpriteAnimation does not exist!", std::filesystem::exists(filepath)));

		const std::string& spriteAnimationName = filepath.stem().string();
		assert(("Error: SpriteAnimation name is already taken!", m_spriteAnimationBank.find(spriteAnimationName) == m_spriteAnimationBank.end()));

		const std::filesystem::path& extension = filepath.extension();

		if (extension == ".spriteanimation") {
			m_spriteAnimationBank[spriteAnimationName] = SpriteAnimationLoader::LoadSpriteAnimation(filepath, this);
			assert(("Error: Could not load SpriteAnimation!", m_spriteAnimationBank[spriteAnimationName]));
		}
		else {
			assert(("Error: Unsupported SpriteAnimation type!", false));
		}

		m_spriteAnimationBank[spriteAnimationName] = SpriteAnimationLoader::LoadSpriteAnimation(filepath, this);
		assert(("Error: Could not load SpriteAnimation!", m_spriteAnimationBank[spriteAnimationName]));
	}

	void ResourceManager::LoadSpriteAnimation(const std::string& filepath)
	{
		std::filesystem::path path(filepath);
		if (path.has_root_directory()) {
			LoadSpriteAnimation(path);
		}
		else {
			std::filesystem::path truePath(s_spriteAnimationDirPath / path);
			LoadSpriteAnimation(truePath);
		}
	}

	void ResourceManager::LoadAllSpriteAnimations()
	{
		for (
			auto it = std::filesystem::recursive_directory_iterator(s_spriteAnimationDirPath);
			it != std::filesystem::recursive_directory_iterator();
			it++
			) {
			if (it->is_regular_file()) {
				LoadSpriteAnimation(it->path());
			}
		}
	}

	SpriteAnimation* ResourceManager::GetSpriteAnimation(std::string spriteAnimationName)
	{
		assert(("Error: SpriteAnimation does not exist!", m_spriteAnimationBank.find(spriteAnimationName) != m_spriteAnimationBank.end()));
		assert(("Error: SpriteAnimation is null!", m_spriteAnimationBank[spriteAnimationName]));

		return m_spriteAnimationBank[spriteAnimationName].get();
	}
}