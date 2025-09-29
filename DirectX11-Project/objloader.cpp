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

Mesh* ObjLoader::LoadMesh(const char* filePath, ID3D11Device* device)
{
	int filePathLength = strlen(filePath);

	// Ensure the file path is long enough to contain ".obj" at the end.
	if (filePathLength <= 4) {
		return 0;
	}

	char last4[4 + 1];
	strcpy_s(last4, &filePath[filePathLength - 4]);

	// Ensure the file extension is .obj
	if (strcmp(last4, ".obj") != 0) {
		return 0;
	}

	FILE* filePtr;
	int error = 0;

	// Open the file for reading.
	error = fopen_s(&filePtr, filePath, "r");
	if (error != 0) {
		return 0;
	}

	std::vector<DirectX::XMFLOAT3> vertexPosition;
	std::vector<DirectX::XMFLOAT3> vertexNormal;
	std::vector<DirectX::XMFLOAT2> vertexTexCoord;

	char line[256];
	while (fgets(line, sizeof(line), filePtr) != 0) {
		if (line[0] == 'v' && line[1] == ' ') {
			char* next;
			float x = strtof(line + 2, &next);
			float y = strtof(next + 1, &next);
			float z = strtof(next + 1, &next);
			vertexPosition.emplace_back(x, y, z);
		}
		else if (line[0] == 'v' && line[1] == 'n' && line[2] == ' ') {
			char* next;
			float x = strtof(line + 3, &next);
			float y = strtof(next + 1, &next);
			float z = strtof(next + 1, &next);
			vertexNormal.emplace_back(x, y, z);
		}
		else if (line[0] == 'v' && line[1] == 't' && line[2] == ' ') {
			char* next;
			float u = strtof(line + 3, &next);
			float v = strtof(next + 1, &next);
			vertexTexCoord.emplace_back(u, v);
		}
	}

	return 0;
}