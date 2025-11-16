#include "mesh.h"

#include <utility>

namespace Engine
{
	Mesh::Mesh(VertexBuffer& vbo, IndexBuffer& ibo, D3D11_PRIMITIVE_TOPOLOGY primitiveType)
		: m_vbo(std::move(vbo)), m_ibo(std::move(ibo)), m_primitiveType(primitiveType)
	{
	}

	void Mesh::Upload(ID3D11Device* device)
	{
		m_vbo.Upload(device);
		m_ibo.Upload(device);
	}

	void Mesh::Release()
	{
		m_vbo.Release();
		m_ibo.Release();
	}

	void Mesh::Bind(ID3D11DeviceContext* deviceContext)
	{
		m_vbo.Bind(deviceContext);
		m_ibo.Bind(deviceContext);
	}

	VertexBuffer& Mesh::GetVBO()
	{
		return m_vbo;
	}

	IndexBuffer& Mesh::GetIBO()
	{
		return m_ibo;
	}
}