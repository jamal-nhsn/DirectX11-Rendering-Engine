#pragma once

/*======
INCLUDES
======*/
#include <stdio.h>

#include "mesh.h"

class ObjLoader
{
public:
	ObjLoader();
	ObjLoader(ObjLoader& other);
	~ObjLoader();

	Mesh* LoadMesh(const char* fileName, ID3D11Device* device);
};