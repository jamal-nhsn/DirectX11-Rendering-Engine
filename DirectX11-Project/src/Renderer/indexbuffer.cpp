#include "pch.h"
#include "indexbuffer.h"

namespace Engine
{
	IndexBuffer::IndexBuffer(const unsigned int* indices, unsigned int indexCount, const D3D11_BUFFER_DESC* bufferDesc)
		: m_ibo(0), m_data(indexCount * sizeof(unsigned int)), m_indexCount(indexCount)
	{
		// Copy the indices as bytes.
		std::memcpy(m_data.data(), indices, m_data.size());

		if (!bufferDesc) {
			m_bufferDesc.Usage = D3D11_USAGE_DEFAULT;
			m_bufferDesc.ByteWidth = sizeof(unsigned int) * indexCount;
			m_bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			m_bufferDesc.CPUAccessFlags = 0;
			m_bufferDesc.MiscFlags = 0;
			m_bufferDesc.StructureByteStride = 0;
		}
		else {
			m_bufferDesc = D3D11_BUFFER_DESC(*bufferDesc);
		}
	}

	IndexBuffer::IndexBuffer(IndexBuffer&& other) noexcept
	{
		// Release what we currently have.
		Release();

		m_ibo = other.m_ibo;
		m_bufferDesc = other.m_bufferDesc;
		m_data.swap(other.m_data);
		m_indexCount = other.m_indexCount;

		// Invalidate other.
		other.m_ibo = 0;
	}

	IndexBuffer& IndexBuffer::operator=(IndexBuffer&& other) noexcept
	{
		// Release what we currently have.
		Release();

		m_ibo = other.m_ibo;
		m_bufferDesc = other.m_bufferDesc;
		m_data.swap(other.m_data);
		m_indexCount = other.m_indexCount;

		// Invalidate other.
		other.m_ibo = 0;

		return *this;
	}

	IndexBuffer::~IndexBuffer()
	{
		if (m_ibo) {
			m_ibo->Release();
		}
	}

	void IndexBuffer::Upload(ID3D11Device* device)
	{
		// Already uploaded.
		if (m_ibo) {
			return;
		}

		D3D11_SUBRESOURCE_DATA indexData;
		indexData.pSysMem = m_data.data();
		indexData.SysMemPitch = 0;
		indexData.SysMemSlicePitch = 0;

		HRESULT result = device->CreateBuffer(&m_bufferDesc, &indexData, &m_ibo);
		assert(("Error: Could not upload IBO to GPU!", !FAILED(result)));
	}

	void IndexBuffer::Release()
	{
		if (m_ibo) {
			m_ibo->Release();
			m_ibo = 0;
		}
	}

	void IndexBuffer::Bind(ID3D11DeviceContext* deviceContext)
	{
		// Ensure ibo was properly created.
		assert(("Error: IBO not uploaded to GPU!", m_ibo));
		deviceContext->IASetIndexBuffer(m_ibo, DXGI_FORMAT_R32_UINT, 0);
	}

	void IndexBuffer::Map(ID3D11DeviceContext* deviceContext, D3D11_MAPPED_SUBRESOURCE* mappedResource, D3D11_MAP mapMode, UINT8 mapFlags)
	{
		HRESULT result = deviceContext->Map(m_ibo, 0, mapMode, mapFlags, mappedResource);
		assert(("Error: Unable to map IBO!", !FAILED(result)));
	}

	void IndexBuffer::Unmap(ID3D11DeviceContext* deviceContext, UINT8 subresource)
	{
		deviceContext->Unmap(m_ibo, subresource);
	}

	unsigned int IndexBuffer::GetIndexCount()
	{
		return m_indexCount;
	}

	const std::vector<char>& IndexBuffer::GetData()
	{
		return m_data;
	}

	const ID3D11Buffer* IndexBuffer::GetBuffer()
	{
		return m_ibo;
	}
}