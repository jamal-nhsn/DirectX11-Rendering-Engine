#include "pch.h"
#include "vertexbuffer.h"

namespace Engine
{
	VertexBuffer::VertexBuffer(const void* vertices, unsigned int vertexSize, unsigned int vertexCount, const D3D11_BUFFER_DESC* bufferDesc)
		: m_vbo(0), m_data(vertexCount * vertexSize), m_vertexSize(vertexSize), m_vertexCount(vertexCount)
	{
		// Copy the vertices as bytes.
		std::memcpy(m_data.data(), vertices, m_data.size());

		if (!bufferDesc) {
			m_bufferDesc.Usage = D3D11_USAGE_DEFAULT;
			m_bufferDesc.ByteWidth = vertexSize * vertexCount;
			m_bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			m_bufferDesc.CPUAccessFlags = 0;
			m_bufferDesc.MiscFlags = 0;
			m_bufferDesc.StructureByteStride = 0;
		}
		else {
			m_bufferDesc = D3D11_BUFFER_DESC(*bufferDesc);
		}
	}

	VertexBuffer::VertexBuffer(VertexBuffer&& other) noexcept
	{
		// Release what we currently have.
		Release();

		m_vbo = other.m_vbo;
		m_bufferDesc = other.m_bufferDesc;
		m_data.swap(other.m_data);
		m_vertexSize = other.m_vertexSize;
		m_vertexCount = other.m_vertexCount;

		// Invalidate other.
		other.m_vbo = 0;
	}

	VertexBuffer& VertexBuffer::operator=(VertexBuffer&& other) noexcept
	{
		// Release what we currently have.
		Release();

		m_vbo = other.m_vbo;
		m_bufferDesc = other.m_bufferDesc;
		m_data.swap(other.m_data);
		m_vertexSize = other.m_vertexSize;
		m_vertexCount = other.m_vertexCount;

		// Invalidate other.
		other.m_vbo = 0;

		return *this;
	}

	VertexBuffer::~VertexBuffer()
	{
		Release();
	}

	void VertexBuffer::Upload(ID3D11Device* device)
	{
		// Already uploaded.
		if (m_vbo) {
			return;
		}

		D3D11_SUBRESOURCE_DATA vertexData;
		vertexData.pSysMem = m_data.data();
		vertexData.SysMemPitch = 0;
		vertexData.SysMemSlicePitch = 0;

		HRESULT result = device->CreateBuffer(&m_bufferDesc, &vertexData, &m_vbo);
		assert(("Error: Could not upload VBO to GPU!", !FAILED(result)));
	}

	void VertexBuffer::Release()
	{
		if (m_vbo) {
			m_vbo->Release();
			m_vbo = 0;
		}
	}

	void VertexBuffer::Bind(ID3D11DeviceContext* deviceContext)
	{
		// Ensure vbo was properly created.
		assert(("Error: VBO not uploaded to GPU!", m_vbo));

		unsigned int stride = m_vertexSize;
		unsigned int offset = 0;

		deviceContext->IASetVertexBuffers(0, 1, &m_vbo, &stride, &offset);
	}

	void VertexBuffer::Map(ID3D11DeviceContext* deviceContext, D3D11_MAPPED_SUBRESOURCE* mappedResource, D3D11_MAP mapMode, UINT8 mapFlags)
	{
		HRESULT result = deviceContext->Map(m_vbo, 0, mapMode, mapFlags, mappedResource);
		assert(("Error: Unable to map VBO!", !FAILED(result)));
	}

	void VertexBuffer::Unmap(ID3D11DeviceContext* deviceContext, UINT8 subresource)
	{
		deviceContext->Unmap(m_vbo, subresource);
	}

	unsigned int VertexBuffer::GetVertexCount()
	{
		return m_vertexCount;
	}

	const std::vector<char>& VertexBuffer::GetData()
	{
		return m_data;
	}

	const ID3D11Buffer* VertexBuffer::GetBuffer()
	{
		return m_vbo;
	}
}