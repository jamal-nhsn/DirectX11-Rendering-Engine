#include "resourcemanager.h"

#include <cassert>
#include <array>
#include <vector>
#include <fstream>

namespace Engine
{
	namespace
	{
		std::unique_ptr<Mesh> GenerateTriangleMesh()
		{
			std::array<Vertex3D, 3> vertices;
			std::array<unsigned int, 3> indices;

			float halfRoot3 = 0.866025388f;

			vertices[0].position = DirectX::XMFLOAT3(-halfRoot3, -0.5f, 0.0f);
			vertices[1].position = DirectX::XMFLOAT3(0.0f, 0.5f, 0.0f);
			vertices[2].position = DirectX::XMFLOAT3(halfRoot3, -0.5f, 0.0f);

			vertices[0].texCoord = DirectX::XMFLOAT2(0.0f, 1.0f);
			vertices[1].texCoord = DirectX::XMFLOAT2(0.5f, 0.0f);
			vertices[2].texCoord = DirectX::XMFLOAT2(1.0f, 1.0f);

			vertices[0].normal = DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f);
			vertices[1].normal = DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f);
			vertices[2].normal = DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f);

			vertices[0].color = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
			vertices[1].color = DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
			vertices[2].color = DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);

			indices[0] = 0; indices[1] = 1; indices[2] = 2;

			VertexBuffer vbo(
				vertices.data(),
				static_cast<unsigned int>(sizeof(Vertex3D)),
				static_cast<unsigned int>(vertices.size())
			);

			IndexBuffer ibo(
				indices.data(),
				static_cast<unsigned int>(indices.size())
			);

			return std::make_unique<Mesh>(vbo, ibo, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		}

		std::unique_ptr<Mesh> GenerateQuadMesh()
		{
			std::array<Vertex3D, 4> vertices;
			std::array<unsigned int, 6> indices;

			vertices[0].position = DirectX::XMFLOAT3(-0.5f, -0.5f, 0.0f);
			vertices[1].position = DirectX::XMFLOAT3(-0.5f, 0.5f, 0.0f);
			vertices[2].position = DirectX::XMFLOAT3(0.5f, 0.5f, 0.0f);
			vertices[3].position = DirectX::XMFLOAT3(0.5f, -0.5f, 0.0f);

			vertices[0].texCoord = DirectX::XMFLOAT2(0.0f, 1.0f);
			vertices[1].texCoord = DirectX::XMFLOAT2(0.0f, 0.0f);
			vertices[2].texCoord = DirectX::XMFLOAT2(1.0f, 0.0f);
			vertices[3].texCoord = DirectX::XMFLOAT2(1.0f, 1.0f);

			vertices[0].normal = DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f);
			vertices[1].normal = DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f);
			vertices[2].normal = DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f);
			vertices[3].normal = DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f);

			vertices[0].color = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
			vertices[1].color = DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
			vertices[2].color = DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
			vertices[3].color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

			indices[0] = 0; indices[1] = 1; indices[2] = 2;
			indices[3] = 0; indices[4] = 2; indices[5] = 3;

			VertexBuffer vbo(
				vertices.data(),
				static_cast<unsigned int>(sizeof(Vertex3D)),
				static_cast<unsigned int>(vertices.size())
			);

			IndexBuffer ibo(
				indices.data(),
				static_cast<unsigned int>(indices.size())
			);

			return std::make_unique<Mesh>(vbo, ibo, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		}

		std::unique_ptr<Mesh> GenerateCubeMesh()
		{
			std::array<Vertex3D, 24> vertices;
			std::array<unsigned int, 36> indices;

			// Front
			vertices[0].position = DirectX::XMFLOAT3(-0.5f, -0.5f, -0.5f);
			vertices[1].position = DirectX::XMFLOAT3(-0.5f, 0.5f, -0.5f);
			vertices[2].position = DirectX::XMFLOAT3(0.5f, 0.5f, -0.5f);
			vertices[3].position = DirectX::XMFLOAT3(0.5f, -0.5f, -0.5f);

			vertices[0].texCoord = DirectX::XMFLOAT2(0.0f, 1.0f);
			vertices[1].texCoord = DirectX::XMFLOAT2(0.0f, 0.0f);
			vertices[2].texCoord = DirectX::XMFLOAT2(1.0f, 0.0f);
			vertices[3].texCoord = DirectX::XMFLOAT2(1.0f, 1.0f);

			vertices[0].normal = DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f);
			vertices[1].normal = DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f);
			vertices[2].normal = DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f);
			vertices[3].normal = DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f);

			vertices[0].color = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
			vertices[1].color = DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
			vertices[2].color = DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
			vertices[3].color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

			indices[0] = 0; indices[1] = 1; indices[2] = 2;
			indices[3] = 0; indices[4] = 2; indices[5] = 3;

			// Back
			vertices[4].position = DirectX::XMFLOAT3(-0.5f, 0.5f, 0.5f);
			vertices[5].position = DirectX::XMFLOAT3(-0.5f, -0.5f, 0.5f);
			vertices[6].position = DirectX::XMFLOAT3(0.5f, -0.5f, 0.5f);
			vertices[7].position = DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f);

			vertices[4].texCoord = DirectX::XMFLOAT2(1.0f, 0.0f);
			vertices[5].texCoord = DirectX::XMFLOAT2(1.0f, 1.0f);
			vertices[6].texCoord = DirectX::XMFLOAT2(0.0f, 1.0f);
			vertices[7].texCoord = DirectX::XMFLOAT2(0.0f, 0.0f);

			vertices[4].normal = DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f);
			vertices[5].normal = DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f);
			vertices[6].normal = DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f);
			vertices[7].normal = DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f);

			vertices[4].color = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
			vertices[5].color = DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
			vertices[6].color = DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
			vertices[7].color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

			indices[6] = 4; indices[7] = 5; indices[8] = 6;
			indices[9] = 4; indices[10] = 6; indices[11] = 7;

			// Right
			vertices[8].position = DirectX::XMFLOAT3(0.5f, -0.5f, -0.5f);
			vertices[9].position = DirectX::XMFLOAT3(0.5f, 0.5f, -0.5f);
			vertices[10].position = DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f);
			vertices[11].position = DirectX::XMFLOAT3(0.5f, -0.5f, 0.5f);

			vertices[8].texCoord = DirectX::XMFLOAT2(0.0f, 1.0f);
			vertices[9].texCoord = DirectX::XMFLOAT2(0.0f, 0.0f);
			vertices[10].texCoord = DirectX::XMFLOAT2(1.0f, 0.0f);
			vertices[11].texCoord = DirectX::XMFLOAT2(1.0f, 1.0f);

			vertices[8].normal = DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f);
			vertices[9].normal = DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f);
			vertices[10].normal = DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f);
			vertices[11].normal = DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f);

			vertices[8].color = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
			vertices[9].color = DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
			vertices[10].color = DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
			vertices[11].color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

			indices[12] = 8; indices[13] = 9; indices[14] = 10;
			indices[15] = 8; indices[16] = 10; indices[17] = 11;

			// Left
			vertices[12].position = DirectX::XMFLOAT3(-0.5f, 0.5f, -0.5f);
			vertices[13].position = DirectX::XMFLOAT3(-0.5f, -0.5f, -0.5f);
			vertices[14].position = DirectX::XMFLOAT3(-0.5f, -0.5f, 0.5f);
			vertices[15].position = DirectX::XMFLOAT3(-0.5f, 0.5f, 0.5f);

			vertices[12].texCoord = DirectX::XMFLOAT2(1.0f, 0.0f);
			vertices[13].texCoord = DirectX::XMFLOAT2(1.0f, 1.0f);
			vertices[14].texCoord = DirectX::XMFLOAT2(0.0f, 1.0f);
			vertices[15].texCoord = DirectX::XMFLOAT2(0.0f, 0.0f);

			vertices[12].normal = DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f);
			vertices[13].normal = DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f);
			vertices[14].normal = DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f);
			vertices[15].normal = DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f);

			vertices[12].color = DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);
			vertices[13].color = DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
			vertices[14].color = DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
			vertices[15].color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

			indices[18] = 12; indices[19] = 13; indices[20] = 14;
			indices[21] = 12; indices[22] = 14; indices[23] = 15;

			// Top
			vertices[16].position = DirectX::XMFLOAT3(-0.5f, 0.5f, -0.5f);
			vertices[17].position = DirectX::XMFLOAT3(-0.5f, 0.5f, 0.5f);
			vertices[18].position = DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f);
			vertices[19].position = DirectX::XMFLOAT3(0.5f, 0.5f, -0.5f);

			vertices[16].texCoord = DirectX::XMFLOAT2(0.0f, 1.0f);
			vertices[17].texCoord = DirectX::XMFLOAT2(0.0f, 0.0f);
			vertices[18].texCoord = DirectX::XMFLOAT2(1.0f, 0.0f);
			vertices[19].texCoord = DirectX::XMFLOAT2(1.0f, 1.0f);

			vertices[16].normal = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);
			vertices[17].normal = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);
			vertices[18].normal = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);
			vertices[19].normal = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);

			vertices[16].color = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
			vertices[17].color = DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
			vertices[18].color = DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
			vertices[19].color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

			indices[24] = 16; indices[25] = 17; indices[26] = 18;
			indices[27] = 16; indices[28] = 18; indices[29] = 19;

			// Bottom
			vertices[20].position = DirectX::XMFLOAT3(0.5f, -0.5f, -0.5f);
			vertices[21].position = DirectX::XMFLOAT3(0.5f, -0.5f, 0.5f);
			vertices[22].position = DirectX::XMFLOAT3(-0.5f, -0.5f, 0.5f);
			vertices[23].position = DirectX::XMFLOAT3(-0.5f, -0.5f, -0.5f);

			vertices[20].texCoord = DirectX::XMFLOAT2(0.0f, 1.0f);
			vertices[21].texCoord = DirectX::XMFLOAT2(0.0f, 0.0f);
			vertices[22].texCoord = DirectX::XMFLOAT2(1.0f, 0.0f);
			vertices[23].texCoord = DirectX::XMFLOAT2(1.0f, 1.0f);

			vertices[20].normal = DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f);
			vertices[21].normal = DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f);
			vertices[22].normal = DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f);
			vertices[23].normal = DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f);

			vertices[20].color = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
			vertices[21].color = DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
			vertices[22].color = DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
			vertices[23].color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

			indices[30] = 20; indices[31] = 21; indices[32] = 22;
			indices[33] = 20; indices[34] = 22; indices[35] = 23;

			VertexBuffer vbo(
				vertices.data(),
				static_cast<unsigned int>(sizeof(Vertex3D)),
				static_cast<unsigned int>(vertices.size())
			);

			IndexBuffer ibo(
				indices.data(),
				static_cast<unsigned int>(indices.size())
			);

			return std::make_unique<Mesh>(vbo, ibo, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		}
	}

	void ResourceManager::MeshLoader::LoadBuiltIn(std::unordered_map<std::string, std::unique_ptr<Mesh>>& meshBank)
	{
		meshBank["triangle"] = GenerateTriangleMesh();
		meshBank["quad"] = GenerateQuadMesh();
		meshBank["cube"] = GenerateCubeMesh();
	}

	std::unique_ptr<Mesh> ResourceManager::MeshLoader::LoadMeshOBJ(const std::filesystem::path& filepath)
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

		return std::make_unique<Mesh>(vbo, ibo, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}

	std::unique_ptr<Mesh> ResourceManager::MeshLoader::LoadMeshBinary(const std::filesystem::path& filepath)
	{
		std::ifstream file(filepath, std::ios::binary);

		if (!file.is_open()) {
			return 0;
		}

		BinaryHeader header;
		file.read(reinterpret_cast<char*>(&header), sizeof(BinaryHeader));

		std::vector<Vertex3D> vertices(header.vertexCount);
		std::vector<unsigned int> indices(header.indexCount);

		file.read(reinterpret_cast<char*>(vertices.data()), sizeof(Vertex3D) * header.vertexCount);
		file.read(reinterpret_cast<char*>(indices.data()), sizeof(unsigned int) * header.indexCount);

		Engine::VertexBuffer vbo(
			vertices.data(),
			static_cast<unsigned int>(sizeof(Vertex3D)),
			static_cast<unsigned int>(vertices.size())
		);

		Engine::IndexBuffer ibo(
			indices.data(),
			static_cast<unsigned int>(indices.size())
		);

		return std::make_unique<Mesh>(vbo, ibo, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}

	void ResourceManager::MeshLoader::CreateBinary(const std::filesystem::path& filepath, Mesh* mesh)
	{
		assert(("Error: Mesh cannot be null!", mesh));

		std::ofstream file(filepath, std::ios::binary);
		assert(("Error: Cannot create mesh binary file!", file.is_open()));

		const std::vector<char>& vertexData = mesh->GetVBO().GetData();
		const std::vector<char>& indexData = mesh->GetIBO().GetData();

		// Create the header.
		BinaryHeader header{
			static_cast<unsigned int>(vertexData.size() / sizeof(Vertex3D)),
			static_cast<unsigned int>(indexData.size() / sizeof(unsigned int))
		};

		// Write the header.
		file.write(reinterpret_cast<const char*>(&header), sizeof(BinaryHeader));

		file.write(vertexData.data(), vertexData.size());
		file.write(indexData.data(), indexData.size());
	}
}