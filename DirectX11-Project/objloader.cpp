#include "objloader.h"

ObjLoader::ObjLoader()
{
}

ObjLoader::ObjLoader(ObjLoader& other)
{
}

ObjLoader::~ObjLoader()
{
}

Mesh* ObjLoader::LoadMesh(const char* fileName, ID3D11Device* device)
{
	int fileNameLength = strlen(fileName);

	// Ensure the file name is long enough to contain ".obj" at the end.
	if (fileNameLength <= 4) {
		return 0;
	}

	char last4[4 + 1];
	strcpy_s(last4, &fileName[fileNameLength - 4]);

	// Ensure the file extension is .obj
	if (!strcmp(last4, ".obj")) {
		return 0;
	}

	return 0;
}