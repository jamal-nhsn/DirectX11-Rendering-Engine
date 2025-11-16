#include "resourceloaders.h"

#include <fstream>

namespace Engine
{
	std::unique_ptr<Mesh> LoadMeshBinary(const std::filesystem::path& filepath, ID3D11Device* device)
	{
		std::ifstream file(filepath, std::ios::binary);

		if (!file.is_open()) {
			return 0;
		}

		struct Header
		{
			unsigned int vertexCount;
			unsigned int indexCount;
		};

		Header header;
		file.read(reinterpret_cast<char*>(&header), sizeof(Header));

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
}