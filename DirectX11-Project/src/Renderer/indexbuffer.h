#pragma once

#include <d3d11.h>

namespace Engine
{
	class IndexBuffer
	{
	public:
		IndexBuffer(ID3D11Device* device, const unsigned int* indices, unsigned int indexCount, const D3D11_BUFFER_DESC* bufferDesc = 0);
		
		// Do not allow copying.
		IndexBuffer(const IndexBuffer& other) = delete;
		IndexBuffer& operator=(const IndexBuffer& other) = delete;
		
		// Allow moving.
		IndexBuffer(IndexBuffer&& other) noexcept;
		IndexBuffer& operator=(IndexBuffer&& other) noexcept;
		
		~IndexBuffer();

		bool Bind(ID3D11DeviceContext* deviceContext);

		bool Map(ID3D11DeviceContext* deviceContext, D3D11_MAPPED_SUBRESOURCE* mappedResource, D3D11_MAP mapMode, UINT8 mapFlags = 0);
		void Unmap(ID3D11DeviceContext* deviceContext, UINT8 subresource = 0);

		unsigned int GetIndexCount();
		const ID3D11Buffer* GetBuffer();

	private:
		ID3D11Buffer* m_ibo;
		unsigned int m_indexCount;
	};
}