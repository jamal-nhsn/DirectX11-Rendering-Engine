#pragma once

#include "objloader.h"

#include <unordered_map>
#include <memory>
#include <string>

class MeshManager
{
public:
	MeshManager(ID3D11Device* device);

	// Shouldn't need to be copied.
	MeshManager(const MeshManager& other) = delete;
	~MeshManager() = default;

	std::shared_ptr<Engine::Mesh> GetMesh(std::string meshName);

private:
	void InitializeTriangle(ID3D11Device* device);
	void InitializeQuad(ID3D11Device* device);
	void InitializeCube(ID3D11Device* device);
	void InitializeObjFiles(ID3D11Device* device);

private:
	std::unordered_map<std::string, std::shared_ptr<Engine::Mesh>> m_meshBank;
};