#include "meshmanager.h"

MeshManager::MeshManager()
{
}

MeshManager::MeshManager(const MeshManager& other)
{
}

MeshManager::~MeshManager()
{
}

bool MeshManager::Initialize(ID3D11Device* device)
{
	bool success = true;

	success = InitializeTriangle(device);
	if (!success) {
		return success;
	}

	success = InitializeQuad(device);
	if (!success) {
		return success;
	}

	return success;
}

Mesh* MeshManager::GetMesh(const char* meshName)
{
	return m_meshBank[meshName];
}

void MeshManager::Shutdown()
{
	for (auto& entry : m_meshBank) {
		entry.second->Shutdown();
		delete entry.second;
	}
}

bool MeshManager::InitializeTriangle(ID3D11Device* device)
{
	int vertexCount = 3;
	int indexCount = 3;

	Vertex* vertices = new Vertex[vertexCount];
	if (!vertices) {
		return false;
	}

	unsigned long* indices = new unsigned long[vertexCount];
	if (!indices) {
		delete[] vertices;
		return false;
	}

	vertices[0].position = DirectX::XMFLOAT3(-1.0f, -1.0f, 0.0f);
	vertices[0].color = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);

	vertices[1].position = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertices[1].color = DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	vertices[2].position = DirectX::XMFLOAT3(1.0f, -1.0f, 0.0f);
	vertices[2].color = DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);

	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;

	Mesh* triangle = new Mesh;
	bool success = triangle->Initialize(device, vertices, vertexCount, indices, indexCount, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	if (success) {
		m_meshBank["triangle"] = triangle;
	}
	else {
		delete triangle;
	}

	delete[] vertices;
	delete[] indices;

	return success;
}

bool MeshManager::InitializeQuad(ID3D11Device* device)
{
	int vertexCount = 4;
	int indexCount = 6;

	Vertex* vertices = new Vertex[vertexCount];
	if (!vertices) {
		return false;
	}

	unsigned long* indices = new unsigned long[indexCount];
	if (!indices) {
		delete[] vertices;
		return false;
	}

	vertices[0].position = DirectX::XMFLOAT3(-1.0f, -1.0f, 0.0f);
	vertices[0].color = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);

	vertices[1].position = DirectX::XMFLOAT3(-1.0f, 1.0f, 0.0f);
	vertices[1].color = DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	vertices[2].position = DirectX::XMFLOAT3(1.0f, 1.0f, 0.0f);
	vertices[2].color = DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);

	vertices[3].position = DirectX::XMFLOAT3(1.0f, -1.0f, 0.0f);
	vertices[3].color = DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);

	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;

	indices[3] = 0;
	indices[4] = 2;
	indices[5] = 3;

	Mesh* quad = new Mesh;
	bool success = quad->Initialize(device, vertices, vertexCount, indices, indexCount, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	if (success) {
		m_meshBank["quad"] = quad;
	}
	else {
		delete quad;
	}

	delete[] vertices;
	delete[] indices;

	return success;
}