#include "indexbuffer.h"

namespace Engine
{
	IndexBuffer::IndexBuffer(ID3D11Device* device, const unsigned int* indices, unsigned int indexCount, const D3D11_BUFFER_DESC* bufferDesc)
		: m_indexCount(indexCount)
	{
		D3D11_SUBRESOURCE_DATA indexData;
		indexData.pSysMem = indices;
		indexData.SysMemPitch = 0;
		indexData.SysMemSlicePitch = 0;

		HRESULT result;

		if (!bufferDesc) {
			D3D11_BUFFER_DESC defaultDesc;
			defaultDesc.Usage = D3D11_USAGE_DEFAULT;
			defaultDesc.ByteWidth = sizeof(unsigned int) * indexCount;
			defaultDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			defaultDesc.CPUAccessFlags = 0;
			defaultDesc.MiscFlags = 0;
			defaultDesc.StructureByteStride = 0;

			result = device->CreateBuffer(&defaultDesc, &indexData, &m_ibo);
		}
		else {
			result = device->CreateBuffer(bufferDesc, &indexData, &m_ibo);
		}

		// Initialization failed, functions called on this buffer will also fail.
		if (FAILED(result)) {
			m_ibo = 0;
			indexCount = 0;
		}
	}

	IndexBuffer::IndexBuffer(IndexBuffer&& other) noexcept
	{
		m_ibo = other.m_ibo;
		m_indexCount = other.m_indexCount;

		// Invalidate other.
		other.m_ibo = 0;
		other.m_indexCount = 0;
	}

	IndexBuffer& IndexBuffer::operator=(IndexBuffer&& other) noexcept
	{
		m_ibo = other.m_ibo;
		m_indexCount = other.m_indexCount;

		// Invalidate other.
		other.m_ibo = 0;
		other.m_indexCount = 0;

		return *this;
	}

	IndexBuffer::~IndexBuffer()
	{
		if (m_ibo) {
			m_ibo->Release();
		}
	}

	bool IndexBuffer::Bind(ID3D11DeviceContext* deviceContext)
	{
		// Ensure ibo was properly created.
		if (!m_ibo) {
			return false;
		}

		deviceContext->IASetIndexBuffer(m_ibo, DXGI_FORMAT_R32_UINT, 0);
		return true;
	}

	bool IndexBuffer::Map(ID3D11DeviceContext* deviceContext, D3D11_MAPPED_SUBRESOURCE* mappedResource, D3D11_MAP mapMode, UINT8 mapFlags)
	{
		HRESULT result = deviceContext->Map(m_ibo, 0, mapMode, mapFlags, mappedResource);
		return !FAILED(result);
	}

	void IndexBuffer::Unmap(ID3D11DeviceContext* deviceContext, UINT8 subresource)
	{
		deviceContext->Unmap(m_ibo, subresource);
	}

	unsigned int IndexBuffer::GetIndexCount()
	{
		return m_indexCount;
	}

	const ID3D11Buffer* IndexBuffer::GetBuffer()
	{
		return m_ibo;
	}
}