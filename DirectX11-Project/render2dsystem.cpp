#include "render2dsystem.h"

Render2DSystem::Render2DSystem()
{
	m_vbo = 0;
}

Render2DSystem::Render2DSystem(const Render2DSystem& other)
{
	m_vbo = other.m_vbo;
}

Render2DSystem::~Render2DSystem()
{
	if (m_vbo) {
		m_vbo->Release();
		m_vbo = 0;
	}
}

bool Render2DSystem::Initialize(ID3D11Device* device)
{
	HRESULT result;

	// Create the dynamic vertex buffer desc allocating the max amount of vertices.
	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(Vertex2D) * 6 * MAX_SPRITE_BATCH_SIZE;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Create the dynamic vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, 0, &m_vbo);

	return FAILED(result);
}

void Render2DSystem::CreateBatches(Scene* scene)
{
	std::vector<Sprite>* sprites = scene->GetComponents<Sprite>();

	for (Sprite& sprite : *sprites) {

		Shader* shader = sprite.GetShader();
		ID3D11ShaderResourceView* texture = sprite.GetTexture()->GetTexture2D();
		Transform& transform = scene->GetComponent<Transform>(sprite.GetEntityId());

		size_t batchIndex;
		size_t textureIndex;
		for (batchIndex = 0; batchIndex < m_batches.size(); batchIndex++) {

			if (m_batches[batchIndex].vertices.size() == 6 * MAX_SPRITE_BATCH_SIZE) {
				continue;
			}

			if (shader != m_batches[batchIndex].shader) {
				continue;
			}

			for (textureIndex = 0; textureIndex < m_batches[batchIndex].textures.size(); textureIndex++) {
				if (texture != m_batches[batchIndex].textures[textureIndex]) {
					continue;
				}
				break;
			}

			if (textureIndex == m_batches[batchIndex].textures.size()) {
				if (m_batches[batchIndex].textures.size() < D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT) {
					m_batches[batchIndex].textures.emplace_back(texture);
				}
				else {
					continue;
				}
			}

			break;
		}

		if (batchIndex == m_batches.size()) {
			m_batches.emplace_back();
			m_batches[batchIndex].shader = shader;
			m_batches[batchIndex].textures.emplace_back(texture);
			textureIndex = 0;
		}

		DirectX::XMMATRIX modelMatrix = transform.GetModelMatrix();
		DirectX::XMMATRIX positions = DirectX::XMMATRIX(
			-0.5f, -0.5f, 0.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 1.0f,
			 0.5f,  0.5f, 0.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 1.0f
		);

		positions = DirectX::XMMatrixMultiply(positions, modelMatrix);
		DirectX::XMFLOAT4 color = sprite.GetTint();
		DirectX::XMFLOAT4 uvBounds = sprite.GetUVBounds();

		Vertex2D bottomLeft;
		DirectX::XMStoreFloat3(&bottomLeft.position, positions.r[0]);
		bottomLeft.color = color;
		bottomLeft.texCoord = DirectX::XMFLOAT2(uvBounds.x, uvBounds.y);
		bottomLeft.textureIndex = static_cast<int>(textureIndex);

		Vertex2D topLeft;
		DirectX::XMStoreFloat3(&topLeft.position, positions.r[1]);
		topLeft.color = color;
		topLeft.texCoord = DirectX::XMFLOAT2(uvBounds.x, uvBounds.w);
		topLeft.textureIndex = static_cast<int>(textureIndex);

		Vertex2D topRight;
		DirectX::XMStoreFloat3(&topRight.position, positions.r[2]);
		topRight.color = color;
		topRight.texCoord = DirectX::XMFLOAT2(uvBounds.z, uvBounds.w);
		topRight.textureIndex = static_cast<int>(textureIndex);

		Vertex2D bottomRight;
		DirectX::XMStoreFloat3(&bottomRight.position, positions.r[3]);
		bottomRight.color = color;
		bottomRight.texCoord = DirectX::XMFLOAT2(uvBounds.z, uvBounds.y);
		bottomRight.textureIndex = static_cast<int>(textureIndex);

		m_batches[batchIndex].vertices.emplace_back(bottomLeft);
		m_batches[batchIndex].vertices.emplace_back(topLeft);
		m_batches[batchIndex].vertices.emplace_back(topRight);
		m_batches[batchIndex].vertices.emplace_back(bottomLeft);
		m_batches[batchIndex].vertices.emplace_back(topRight);
		m_batches[batchIndex].vertices.emplace_back(bottomRight);
	}
}

void Render2DSystem::RenderBatches(Direct3D* direct3d, Camera2D& camera)
{
	HRESULT result;
	ID3D11DeviceContext* deviceContext = direct3d->GetDeviceContext();

	unsigned int stride = sizeof(Vertex2D);
	unsigned int offset = 0;

	for (Batch& batch : m_batches) {

		D3D11_MAPPED_SUBRESOURCE mappedResource;
		result = deviceContext->Map(m_vbo, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		if (FAILED(result))
		{
			return;
		}

		Vertex2D* dataPtr = (Vertex2D*)mappedResource.pData;
		memcpy(dataPtr, (void*)batch.vertices.data(), (sizeof(Vertex2D) * batch.vertices.size()));

		deviceContext->Unmap(m_vbo, 0);

		deviceContext->IASetVertexBuffers(0, 1, &m_vbo, &stride, &offset);
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		deviceContext->PSSetShaderResources(0, static_cast<unsigned int>(batch.textures.size()), batch.textures.data());

		deviceContext->Draw(static_cast<unsigned int>(batch.vertices.size()), 0);
	}
}

void Render2DSystem::ClearBatches()
{
	for (Batch& batch : m_batches) {
		batch.vertices.clear();
	}
}

void Render2DSystem::Update(Direct3D* direct3d, Scene* scene)
{
	std::vector<Camera2D>* cameras = scene->GetComponents<Camera2D>();

	CreateBatches(scene);
	for (Camera2D& camera : *cameras) {
		RenderBatches(direct3d, camera);
	}
	ClearBatches();
}

