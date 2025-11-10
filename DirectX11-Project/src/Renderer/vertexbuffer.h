#pragma once

#include "vertex.h"

#include <d3d11.h>

namespace Engine
{
	class VertexBuffer
	{
	public:
		VertexBuffer(ID3D11Device* device, const void* vertices, unsigned int vertexSize, unsigned int vertexCount, const D3D11_BUFFER_DESC* bufferDesc = 0);

		// Do not allow copying.
		VertexBuffer(const VertexBuffer& other) = delete;
		VertexBuffer& operator=(const VertexBuffer& other) = delete;
		
		// Allow moving.
		VertexBuffer(VertexBuffer&& other) noexcept;
		VertexBuffer& operator=(VertexBuffer&& other) noexcept;

		~VertexBuffer();

		bool Bind(ID3D11DeviceContext* deviceContext);

		bool Map(ID3D11DeviceContext* deviceContext, D3D11_MAPPED_SUBRESOURCE* mappedResource, D3D11_MAP mapMode, UINT8 mapFlags = 0);
		void Unmap(ID3D11DeviceContext* deviceContext, UINT8 subresource = 0);

		unsigned int GetVertexCount();

		const ID3D11Buffer* GetBuffer();

	private:
		ID3D11Buffer* m_vbo;
		unsigned int m_vertexSize;
		unsigned int m_vertexCount;
	};
}