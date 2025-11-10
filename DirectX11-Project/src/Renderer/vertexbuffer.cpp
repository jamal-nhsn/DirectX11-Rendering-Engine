#include "vertexbuffer.h"

namespace Engine
{
	VertexBuffer::VertexBuffer(ID3D11Device* device, const void* vertices, unsigned int vertexSize, unsigned int vertexCount, const D3D11_BUFFER_DESC* bufferDesc)
		: m_vertexCount(vertexCount), m_vertexSize(vertexSize)
	{
		D3D11_SUBRESOURCE_DATA vertexData;
		vertexData.pSysMem = vertices;
		vertexData.SysMemPitch = 0;
		vertexData.SysMemSlicePitch = 0;

		HRESULT result;

		if (!bufferDesc) {
			D3D11_BUFFER_DESC defaultDesc;
			defaultDesc.Usage = D3D11_USAGE_DEFAULT;
			defaultDesc.ByteWidth = vertexSize * vertexCount;
			defaultDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			defaultDesc.CPUAccessFlags = 0;
			defaultDesc.MiscFlags = 0;
			defaultDesc.StructureByteStride = 0;

			result = device->CreateBuffer(&defaultDesc, &vertexData, &m_vbo);
		}
		else {
			result = device->CreateBuffer(bufferDesc, &vertexData, &m_vbo);
		}

		// Initialization failed, functions called on this buffer will also fail.
		if (FAILED(result)) {
			m_vbo = 0;
			m_vertexCount = 0;
		}
	}

	VertexBuffer::VertexBuffer(VertexBuffer&& other) noexcept
	{
		m_vbo = other.m_vbo;
		m_vertexCount = other.m_vertexCount;
		m_vertexSize = other.m_vertexSize;

		// Invalidate other.
		other.m_vbo = 0;
		other.m_vertexCount = 0;
		other.m_vertexSize = 0;
	}

	VertexBuffer& VertexBuffer::operator=(VertexBuffer&& other) noexcept
	{
		m_vbo = other.m_vbo;
		m_vertexCount = other.m_vertexCount;
		m_vertexSize = other.m_vertexSize;

		// Invalidate other.
		other.m_vbo = 0;
		other.m_vertexCount = 0;
		other.m_vertexSize = 0;

		return *this;
	}

	VertexBuffer::~VertexBuffer()
	{
		if (m_vbo) {
			m_vbo->Release();
		}
	}

	bool VertexBuffer::Bind(ID3D11DeviceContext* deviceContext)
	{
		// Ensure vbo was properly created.
		if (!m_vbo) {
			return false;
		}

		unsigned int stride = m_vertexSize;
		unsigned int offset = 0;

		deviceContext->IASetVertexBuffers(0, 1, &m_vbo, &stride, &offset);
		return true;
	}

	bool VertexBuffer::Map(ID3D11DeviceContext* deviceContext, D3D11_MAPPED_SUBRESOURCE* mappedResource, D3D11_MAP mapMode, UINT8 mapFlags)
	{
		HRESULT result = deviceContext->Map(m_vbo, 0, mapMode, mapFlags, mappedResource);
		return !FAILED(result);
	}

	void VertexBuffer::Unmap(ID3D11DeviceContext* deviceContext, UINT8 subresource)
	{
		deviceContext->Unmap(m_vbo, subresource);
	}

	unsigned int VertexBuffer::GetVertexCount()
	{
		return m_vertexCount;
	}

	const ID3D11Buffer* VertexBuffer::GetBuffer()
	{
		return m_vbo;
	}
}