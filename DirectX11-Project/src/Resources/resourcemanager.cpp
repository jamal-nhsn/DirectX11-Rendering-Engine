#include "resourcemanager.h"

namespace Engine
{
	ResourceManager::ResourceManager(bool useBinaries)
		: m_useBinaries(useBinaries)
	{
		MeshLoader::LoadBuiltIn(m_meshBank);

		// Create the file hierarchy.
		std::filesystem::create_directory(s_resourceDirPath);

		std::filesystem::create_directory(s_meshDirPath);
		std::filesystem::create_directory(s_spriteAnimationDirPath);
		std::filesystem::create_directory(s_textureDirPath);
		std::filesystem::create_directory(s_textureMetaDirPath);

		std::filesystem::create_directory(s_meshBinaryPath);
		std::filesystem::create_directory(s_spriteAnimationBinaryPath);
		std::filesystem::create_directory(s_textureBinaryPath);
		std::filesystem::create_directory(s_textureMetaBinaryPath);
	}

	void ResourceManager::LoadMesh(const std::filesystem::path& filepath)
	{
		assert(("Error: Mesh does not exist!", std::filesystem::exists(filepath)));

		const std::string& meshName = filepath.stem().string();
		assert(("Error: Mesh name is already taken!", m_meshBank.find(meshName) == m_meshBank.end()));

		// Create path to binary file.
		std::filesystem::path binaryPath(s_meshBinaryPath);
		binaryPath /= meshName + ".bin";

		if (m_useBinaries) {
			// Only use binary file if it is newer.
			bool shouldUseBinary =
				std::filesystem::exists(binaryPath) &&
				std::filesystem::last_write_time(binaryPath) > std::filesystem::last_write_time(filepath);

			if (shouldUseBinary) {
				m_meshBank[meshName] = MeshLoader::LoadMeshBinary(binaryPath);
				assert(("Error: Could not load Mesh!", m_meshBank[meshName]));
				return;
			}
		}

		const std::filesystem::path& extension = filepath.extension();

		if (extension == ".meshobj") {
			m_meshBank[meshName] = MeshLoader::LoadMeshOBJ(filepath);
			assert(("Error: Could not load Mesh!", m_meshBank[meshName]));
		}
		else {
			assert(("Error: Unsupported mesh type!", false));
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
			std::filesystem::path truePath(s_textureDirPath / path);
			LoadMesh(truePath);
		}
	}

	void ResourceManager::LoadAllMeshes()
	{
		std::filesystem::path binaryPath(s_meshBinaryPath);

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
}