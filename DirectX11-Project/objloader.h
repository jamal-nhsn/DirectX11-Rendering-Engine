#pragma once

/*======
INCLUDES
======*/
#include <stdio.h>
#include <vector>
#include <unordered_map>

#include "mesh.h"

class ObjLoader
{
public:
	ObjLoader();
	ObjLoader(ObjLoader& other);
	~ObjLoader();

	Mesh* LoadMesh(const char* filePath, ID3D11Device* device);
};