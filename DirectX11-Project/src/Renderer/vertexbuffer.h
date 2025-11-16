#pragma once

#include "vertex.h"

#include <cassert>
#include <vector>
#include <d3d11.h>

namespace Engine
{
	class VertexBuffer
	{
	public:
		VertexBuffer(const void* vertices, unsigned int vertexSize, unsigned int vertexCount, const D3D11_BUFFER_DESC* bufferDesc = 0);

		// Do not allow copying.
		VertexBuffer(const VertexBuffer& other) = delete;
		VertexBuffer& operator=(const VertexBuffer& other) = delete;
		
		// Allow moving.
		VertexBuffer(VertexBuffer&& other) noexcept;
		VertexBuffer& operator=(VertexBuffer&& other) noexcept;

		~VertexBuffer();

		void Upload(ID3D11Device* device);
		void Release();

		void Bind(ID3D11DeviceContext* deviceContext);

		void Map(ID3D11DeviceContext* deviceContext, D3D11_MAPPED_SUBRESOURCE* mappedResource, D3D11_MAP mapMode, UINT8 mapFlags = 0);
		void Unmap(ID3D11DeviceContext* deviceContext, UINT8 subresource = 0);

		unsigned int GetVertexCount();

		const std::vector<char>& GetData();
		const ID3D11Buffer* GetBuffer();

	private:
		ID3D11Buffer* m_vbo;
		D3D11_BUFFER_DESC m_bufferDesc;

		std::vector<char> m_data;
		unsigned int m_vertexCount;
		unsigned int m_vertexSize;
	};
}