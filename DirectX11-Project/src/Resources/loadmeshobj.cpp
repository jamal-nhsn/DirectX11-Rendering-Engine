#include "pch.h"
#include "resourceloaders.h"

namespace Engine
{
	namespace
	{
		void CreateBinaryFile(const std::filesystem::path& filepath, const std::vector<Vertex3D>& vertices, const std::vector<unsigned int>& indices)
		{
			std::filesystem::path binaryFilepath("Resources/Meshes/.meshbinaries");
			binaryFilepath /= filepath.stem().string() + ".meshbinary";

			std::ofstream file(binaryFilepath, std::ios::binary);
			if (!file.is_open()) {
				return;
			}

			struct Header
			{
				unsigned int vertexCount;
				unsigned int indexCount;
			};

			Header header{ static_cast<unsigned int>(vertices.size()), static_cast<unsigned int>(indices.size()) };
			file.write(reinterpret_cast<const char*>(&header), sizeof(Header));

			const char* vertexData = reinterpret_cast<const char*>(vertices.data());
			const char* indexData = reinterpret_cast<const char*>(indices.data());

			file.write(vertexData, sizeof(Vertex3D) * vertices.size());
			file.write(indexData, sizeof(unsigned int) * indices.size());
		}
	}

	std::unique_ptr<Mesh> LoadMeshOBJ(const std::filesystem::path& filepath, ID3D11Device* device)
	{
		// Invalid filepath.
		assert(("Error: File passed to LoadMeshOBJ is not a .meshobj file!", filepath.extension() == ".meshobj"));

		std::ifstream file(filepath);
		if (!file.is_open()) {
			return 0;
		}

		std::vector<DirectX::XMFLOAT3> vertexPosition;
		std::vector<DirectX::XMFLOAT2> vertexTexCoord;
		std::vector<DirectX::XMFLOAT3> vertexNormal;

		std::vector<Vertex3D> vertices;
		std::vector<unsigned int> indices;

		std::unordered_map<uint64_t, int> vertexKeyToIndex;

		std::string line;
		while (std::getline(file, line)) {
			
			// Vertex position.
			if (line[0] == 'v' && line[1] == ' ') {
				char* next;
				float x = strtof(&line[2], &next);
				float y = strtof(next + 1, &next);
				float z = strtof(next + 1, &next);
				vertexPosition.emplace_back(x, y, z);
			}
			// Vertex normal.
			else if (line[0] == 'v' && line[1] == 'n') {
				char* next;
				float x = strtof(&line[3], &next);
				float y = strtof(next + 1, &next);
				float z = strtof(next + 1, &next);
				vertexNormal.emplace_back(x, y, z);
			}
			// Vertex texcoord.
			else if (line[0] == 'v' && line[1] == 't') {
				char* next;
				float u = strtof(&line[3], &next);
				float v = strtof(next + 1, &next);
				vertexTexCoord.emplace_back(u, v);
			}
			// Process face.
			else if (line[0] == 'f' && line[1] == ' ')
			{
				unsigned int vertexCount = 0;
				for (unsigned int i = 0; line[i] != '\0'; i++) {
					vertexCount += line[i] == ' ' && line[i + 1] != '\n' && line[i + 1] != '\0' && line[i + 1] != '\r' ? 1 : 0;
				}

				char* next = &line[2];
				for (unsigned int vertexNo = 0; vertexNo < vertexCount; vertexNo++) {
					// Read in the vertex data indices.
					int vpi = strtol(next, &next, 10);
					
					// File may have no texcoords.
					int vti = strtol(next + 1, &next, 10);
					next += (vti == 0) * 2;
					
					// File may have no normals.
					int vni = strtol(next + 1, &next, 10);
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
						((uint64_t)(vpi & 0x1FFFFF) << (64 - 21)) |
						((uint64_t)(vti & 0x1FFFFF) << (64 - 42)) |
						((uint64_t)(vni & 0x1FFFFF) << (64 - 63));

					// Create vertex if it doesn't exist yet.
					if (vertexKeyToIndex.find(vertex) == vertexKeyToIndex.end()) {
						int index = static_cast<int>(vertices.size());
						vertices.emplace_back();
						vertices[index].position = vertexPosition[vpi];
						vertices[index].texCoord = vertexTexCoord[vti];
						vertices[index].normal = vertexNormal[vni];

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

		// Create the Buffers.
		VertexBuffer vbo(
			vertices.data(),
			static_cast<unsigned int>(sizeof(Vertex3D)),
			static_cast<unsigned int>(vertices.size())
		);

		IndexBuffer ibo(
			indices.data(),
			static_cast<unsigned int>(indices.size())
		);

		CreateBinaryFile(filepath, vertices, indices);

		return std::make_unique<Mesh>(vbo, ibo, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}
}