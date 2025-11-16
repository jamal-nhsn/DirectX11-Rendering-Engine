#pragma once

#include "vertexbuffer.h"
#include "indexbuffer.h"

namespace Engine
{
	class Mesh
	{
	public:
		Mesh(VertexBuffer& vbo, IndexBuffer& ibo, D3D11_PRIMITIVE_TOPOLOGY primitiveType);
		
		// Do not allow copying.
		Mesh(const Mesh& other) = delete;
		Mesh& operator=(const Mesh& other) = delete;

		// Allow moving.
		Mesh(Mesh&& other) = default;
		Mesh& operator=(Mesh&& other) = default;

		~Mesh() = default;

		void Upload(ID3D11Device* device);
		void Release();

		void Bind(ID3D11DeviceContext* deviceContext);

		VertexBuffer& GetVBO();
		IndexBuffer& GetIBO();

	private:
		VertexBuffer m_vbo;
		IndexBuffer m_ibo;
		D3D_PRIMITIVE_TOPOLOGY m_primitiveType;
	};
}