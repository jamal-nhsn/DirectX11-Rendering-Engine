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

Mesh* ObjLoader::LoadMesh(const char* filePath, ID3D11Device* device, float uScale, float vScale)
{
	int filePathLength = static_cast<int>(strlen(filePath));

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
	std::vector<DirectX::XMFLOAT2> vertexTexCoord;
	std::vector<DirectX::XMFLOAT3> vertexNormal;

	std::vector<Vertex> vertices;
	std::vector<unsigned long>   indices;
	
	std::unordered_map<uint64_t, int> vertexKeyToIndex;

	char line[256];
	while (fgets(line, sizeof(line), filePtr) != 0) {
		// Read in vertex position.
		if (line[0] == 'v' && line[1] == ' ') {
			char* next;
			float x = strtof(line + 2, &next);
			float y = strtof(next + 1, &next);
			float z = strtof(next + 1, &next);
			vertexPosition.emplace_back(x, y, z);
		}
		// Read in vertex texture coordinate.
		else if (line[0] == 'v' && line[1] == 't' && line[2] == ' ') {
			char* next;
			float u = uScale * strtof(line + 3, &next);
			float v = vScale * strtof(next + 1, &next);

			// Flip UV properly
			// Negative UVs do work but keeping them positive is more consistent.
			u += u < 0.0f ? uScale : 0.0f;
			v += v < 0.0f ? vScale : 0.0f;

			vertexTexCoord.emplace_back(u, v);
		}
		// Read in vertex normal.
		else if (line[0] == 'v' && line[1] == 'n' && line[2] == ' ') {
			char* next;
			float x = strtof(line + 3, &next);
			float y = strtof(next + 1, &next);
			float z = strtof(next + 1, &next);
			vertexNormal.emplace_back(x, y, z);
		}
		// Process face.
		else if (line[0] == 'f' && line[1] == ' ')
		{
			int vertexCount = 0;
			for (int i = 0; line[i] != '\0'; i++) {
				vertexCount += line[i] == ' ' && line[i + 1] != '\n' && line[i + 1] != '\0' && line[i + 1] != '\r' ? 1 : 0;
			}

			char* next = line + 2;
			for (int vertexNo = 0; vertexNo < vertexCount; vertexNo++) {
				// Read in the vertex data indices.
				int vpi = strtol(next, &next, 10);
				int vti = strtol(next + 1, &next, 10);
				// File may have no texcoords.
				next += (vti == 0) * 2;
				int vni = strtol(next + 1, &next, 10);
				// File may have no normals.
				next += (vni == 0) * 2;

				// Skip space.
				next++;

				// Indices start from 1 and can be negative to refer to last added value.
				vpi += vpi < 0 ? static_cast<int>(vertexPosition.size()) : -1;
				vti += vti < 0 ? static_cast<int>(vertexTexCoord.size()) : -1;
				vni += vni < 0 ? static_cast<int>(vertexNormal.size()) : -1;

				// Create key into vertex indices.
				// This limits the vertex count to 2^21 vertices or 2,097,152 vertices for now. 
				uint64_t vertex =
					((uint64_t)(vpi & 0x1FFFFF) << (64-21)) |
					((uint64_t)(vti & 0x1FFFFF) << (64-42)) |
					((uint64_t)(vni & 0x1FFFFF) << (64-63));

				// Create vertex if it doesn't exist yet.
				if (vertexKeyToIndex.find(vertex) == vertexKeyToIndex.end()) {
					int index = static_cast<int>(vertices.size());
					vertices.emplace_back();
					vertices[index].position = vertexPosition[vpi];
					vertices[index].texCoord = vertexTexCoord[vti];
					vertices[index].normal   = vertexNormal[vni];

					vertexKeyToIndex[vertex] = index;
				}

				// Triangularize faces as they may n-gons.
				if (vertexNo >= 3) {
					int noIndices = static_cast<int>(indices.size());
					indices.emplace_back(indices[noIndices - vertexNo]);
					indices.emplace_back(indices[noIndices - 1]);
				}

				// Add vertex index to indices.
				indices.emplace_back(vertexKeyToIndex[vertex]);
			}
		}
	}

	// Create the mesh.
	Mesh* mesh = new Mesh;
	bool success = mesh->Initialize(device, vertices.data(), static_cast<int>(vertices.size()), indices.data(), static_cast<int>(indices.size()), D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	if (!success) {
		delete mesh;
		mesh = 0;
	}

	// Close the file
	fclose(filePtr);

	return mesh;
}