#pragma once

#include "../Renderer/mesh.h"

#include <string>
#include <memory>
#include <unordered_map>

namespace Engine
{
	class MeshManager
	{
	public:
		MeshManager(ID3D11Device* device);

		// Shouldn't need to be copied.
		MeshManager(const MeshManager& other) = delete;
		~MeshManager() = default;

		Mesh* GetMesh(std::string meshName);

	private:
		std::unordered_map<std::string, std::unique_ptr<Mesh>> m_meshBank;
	};
}
