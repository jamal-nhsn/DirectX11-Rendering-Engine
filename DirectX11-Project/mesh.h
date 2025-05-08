#pragma once

/*======
INCLUDES
======*/
#include <d3d11.h>

#include "vertex.h"

class Mesh
{
public:
	Mesh();
	Mesh(const Mesh&);
	~Mesh();

	bool Initialize(ID3D11Device* device, Vertex* vertices, int vertexCount, unsigned long* indices, int indexCount);
	void Bind(ID3D11DeviceContext* deviceContext);
	void Shutdown();

private:
	ID3D11Buffer* m_vbo;
	ID3D11Buffer* m_ibo;

	int m_vertexCount;
	int m_indexCount;
};