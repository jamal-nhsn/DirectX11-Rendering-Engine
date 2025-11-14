#pragma once

/*======
INCLUDES
======*/
#include "src/Renderer/mesh.h"

#include <stdio.h>
#include <vector>
#include <unordered_map>
#include <memory>
#include <string>

class ObjLoader
{
public:
	ObjLoader() = default;
	// Shouldn't need to be copied.
	ObjLoader(ObjLoader& other) = delete;
	~ObjLoader() = default;

	std::unique_ptr<Engine::Mesh> LoadMesh(std::string filePath, ID3D11Device* device, float uScale, float vScale);
};