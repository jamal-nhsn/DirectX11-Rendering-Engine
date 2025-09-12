#pragma once

/*======
INCLUDES
======*/
#include <unordered_map>

#include "mesh.h"

class MeshManager
{
public:
	MeshManager();
	MeshManager(const MeshManager&);
	~MeshManager();

	bool Initialize(ID3D11Device* device);
	Mesh* GetMesh(const char* meshName);
	void Shutdown();

private:
	bool InitializeTriangle(ID3D11Device* device);
	bool InitializeQuad(ID3D11Device* device);
	bool InitializeCube(ID3D11Device* device);

private:
	std::unordered_map<const char*, Mesh*> m_meshBank;
};