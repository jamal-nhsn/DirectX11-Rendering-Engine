#include "meshmanager.h"
#include "resourceloaders.h"

#include <array>
#include <cassert>

namespace Engine
{
	namespace
	{
		std::unique_ptr<Mesh> GenerateTriangleMesh(ID3D11Device* device)
		{
			std::array<Vertex3D, 3> vertices;
			std::array<unsigned int, 3> indices;

			float halfRoot3 = 0.866025388f;

			vertices[0].position = DirectX::XMFLOAT3(-halfRoot3, -0.5f, 0.0f);
			vertices[1].position = DirectX::XMFLOAT3(      0.0f,  0.5f, 0.0f);
			vertices[2].position = DirectX::XMFLOAT3( halfRoot3, -0.5f, 0.0f);

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
				device,
				vertices.data(),
				static_cast<unsigned int>(sizeof(Vertex3D)),
				static_cast<unsigned int>(vertices.size())
			);

			IndexBuffer ibo(
				device,
				indices.data(),
				static_cast<unsigned int>(indices.size())
			);

			return std::make_unique<Mesh>(vbo, ibo, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		}

		std::unique_ptr<Mesh> GenerateQuadMesh(ID3D11Device* device)
		{
			std::array<Vertex3D, 4> vertices;
			std::array<unsigned int, 6> indices;

			vertices[0].position = DirectX::XMFLOAT3(-0.5f, -0.5f, 0.0f);
			vertices[1].position = DirectX::XMFLOAT3(-0.5f,  0.5f, 0.0f);
			vertices[2].position = DirectX::XMFLOAT3( 0.5f,  0.5f, 0.0f);
			vertices[3].position = DirectX::XMFLOAT3( 0.5f, -0.5f, 0.0f);

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
				device,
				vertices.data(),
				static_cast<unsigned int>(sizeof(Vertex3D)),
				static_cast<unsigned int>(vertices.size())
			);

			IndexBuffer ibo(
				device,
				indices.data(),
				static_cast<unsigned int>(indices.size())
			);

			return std::make_unique<Mesh>(vbo, ibo, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		}

		std::unique_ptr<Mesh> GenerateCubeMesh(ID3D11Device* device)
		{
			std::array<Vertex3D, 24> vertices;
			std::array<unsigned int, 36> indices;
	
			// Front
			vertices[0].position = DirectX::XMFLOAT3(-0.5f, -0.5f, -0.5f);
			vertices[1].position = DirectX::XMFLOAT3(-0.5f,  0.5f, -0.5f);
			vertices[2].position = DirectX::XMFLOAT3( 0.5f,  0.5f, -0.5f);
			vertices[3].position = DirectX::XMFLOAT3( 0.5f, -0.5f, -0.5f);

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
			vertices[4].position = DirectX::XMFLOAT3(-0.5f,  0.5f, 0.5f);
			vertices[5].position = DirectX::XMFLOAT3(-0.5f, -0.5f, 0.5f);
			vertices[6].position = DirectX::XMFLOAT3( 0.5f, -0.5f, 0.5f);
			vertices[7].position = DirectX::XMFLOAT3( 0.5f,  0.5f, 0.5f);

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

			indices[6] = 4; indices[7]  = 5; indices[8]  = 6;
			indices[9] = 4; indices[10] = 6; indices[11] = 7;

			// Right
			vertices[8].position  = DirectX::XMFLOAT3(0.5f, -0.5f, -0.5f);
			vertices[9].position  = DirectX::XMFLOAT3(0.5f,  0.5f, -0.5f);
			vertices[10].position = DirectX::XMFLOAT3(0.5f,  0.5f,  0.5f);
			vertices[11].position = DirectX::XMFLOAT3(0.5f, -0.5f,  0.5f);

			vertices[8].texCoord  = DirectX::XMFLOAT2(0.0f, 1.0f);
			vertices[9].texCoord  = DirectX::XMFLOAT2(0.0f, 0.0f);
			vertices[10].texCoord = DirectX::XMFLOAT2(1.0f, 0.0f);
			vertices[11].texCoord = DirectX::XMFLOAT2(1.0f, 1.0f);

			vertices[8].normal  = DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f);
			vertices[9].normal  = DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f);
			vertices[10].normal = DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f);
			vertices[11].normal = DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f);

			vertices[8].color  = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
			vertices[9].color  = DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
			vertices[10].color = DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
			vertices[11].color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

			indices[12] = 8; indices[13] = 9; indices[14]  = 10;
			indices[15] = 8; indices[16] = 10; indices[17] = 11;

			// Left
			vertices[12].position = DirectX::XMFLOAT3(-0.5f,  0.5f, -0.5f);
			vertices[13].position = DirectX::XMFLOAT3(-0.5f, -0.5f, -0.5f);
			vertices[14].position = DirectX::XMFLOAT3(-0.5f, -0.5f,  0.5f);
			vertices[15].position = DirectX::XMFLOAT3(-0.5f,  0.5f,  0.5f);

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
			vertices[17].position = DirectX::XMFLOAT3(-0.5f, 0.5f,  0.5f);
			vertices[18].position = DirectX::XMFLOAT3( 0.5f, 0.5f,  0.5f);
			vertices[19].position = DirectX::XMFLOAT3( 0.5f, 0.5f, -0.5f);

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
			vertices[20].position = DirectX::XMFLOAT3( 0.5f, -0.5f, -0.5f);
			vertices[21].position = DirectX::XMFLOAT3( 0.5f, -0.5f,  0.5f);
			vertices[22].position = DirectX::XMFLOAT3(-0.5f, -0.5f,  0.5f);
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
				device,
				vertices.data(),
				static_cast<unsigned int>(sizeof(Vertex3D)),
				static_cast<unsigned int>(vertices.size())
			);

			IndexBuffer ibo(
				device,
				indices.data(),
				static_cast<unsigned int>(indices.size())
			);

			return std::make_unique<Mesh>(vbo, ibo, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		}
	}

	MeshManager::MeshManager(ID3D11Device* device)
	{
		// Built-in meshes.
		m_meshBank["triangle"] = GenerateTriangleMesh(device);
		m_meshBank["quad"] = GenerateQuadMesh(device);
		m_meshBank["cube"] = GenerateCubeMesh(device);

		// Create the mesh directory if it doesn't exist.
		std::filesystem::path meshDirectory("Resources/Meshes");
		std::filesystem::create_directory(meshDirectory);

		// Create the .meshbinaries directory if it doesn't exist.
		std::filesystem::path binariesDirectory("Resources/Meshes/.meshbinaries");
		std::filesystem::create_directory(binariesDirectory);

		// Load all the meshes in the mesh directory.
		for (const auto& entry : std::filesystem::recursive_directory_iterator(meshDirectory))
		{
			// Ignore folders.
			if (entry.is_directory()) {
				continue;
			}

			// Ignore .meshbinary directory.
			if (entry.path().parent_path() == binariesDirectory) {
				continue;
			}

			// Get the path to the mesh itself.
			const std::filesystem::path& filepath = entry.path();

			// Ensure the mesh doesn't already exist.
			const std::string& meshName = filepath.stem().string();
			assert(("Error: Mesh name is already reserved!", m_meshBank.find(meshName) == m_meshBank.end()));

			// Get the path to the corresponding binary file.
			std::filesystem::path binaryPath(binariesDirectory / (meshName + ".meshbinary"));

			// Only use the binary file if it exists and is newer than the standard file.
			bool binaryExists = std::filesystem::exists(binaryPath);
			bool useBinary = binaryExists && std::filesystem::last_write_time(binaryPath) > std::filesystem::last_write_time(filepath);

			if (useBinary) {
				m_meshBank[meshName] = LoadMeshBinary(binaryPath, device);
			}
			else if (filepath.extension() == ".meshobj") {
				m_meshBank[meshName] = LoadMeshOBJ(filepath, device);
			}
			// Only support meshobj for now.
			else {
				assert(("Error: Unsupported mesh type!", false));
			}
		}
	}

	Mesh* MeshManager::GetMesh(std::string meshName)
	{
		assert(("Error: Mesh does not exist!", m_meshBank.find(meshName) != m_meshBank.end()));
		assert(("Error: Mesh is null!", m_meshBank[meshName]));

		return m_meshBank[meshName].get();
	}
}