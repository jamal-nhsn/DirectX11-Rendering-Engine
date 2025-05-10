#include "mesh.h"

Mesh::Mesh()
{
	m_vbo = 0;
	m_ibo = 0;

	m_vertexCount = 0;
	m_indexCount = 0;
}

Mesh::Mesh(const Mesh& other)
{
}

Mesh::~Mesh()
{
}

bool Mesh::Initialize(ID3D11Device* device, Vertex* vertices, int vertexCount, unsigned long* indices, int indexCount, D3D_PRIMITIVE_TOPOLOGY primitiveType)
{
	m_vertexCount = vertexCount;
	m_indexCount  = indexCount;

	m_primitiveType = primitiveType;

	HRESULT result;

	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage               = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth           = sizeof(Vertex) * vertexCount;
	vertexBufferDesc.BindFlags           = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags      = 0;
	vertexBufferDesc.MiscFlags           = 0;
	vertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem          = vertices;
	vertexData.SysMemPitch      = 0;
	vertexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vbo);
	if (FAILED(result)) {
		return false;
	}

	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage               = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth           = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags           = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags      = 0;
	indexBufferDesc.MiscFlags           = 0;
	indexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem          = indices;
	indexData.SysMemPitch      = 0;
	indexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_ibo);

	return !FAILED(result);
}

void Mesh::Bind(ID3D11DeviceContext* deviceContext)
{
	// Set vertex buffer stride and offset.
	unsigned int stride = sizeof(Vertex);
	unsigned int offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &m_vbo, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_ibo, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer.
	deviceContext->IASetPrimitiveTopology(m_primitiveType);
}

void Mesh::Shutdown()
{
	// Release the vbo.
	if (m_vbo) {
		m_vbo->Release();
		m_vbo = 0;
	}

	// Release the ibo.
	if (m_ibo) {
		m_ibo->Release();
		m_ibo = 0;
	}
}