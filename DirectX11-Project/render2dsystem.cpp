#include "render2dsystem.h"

Render2DSystem::Render2DSystem()
{
	m_vbo = 0;
}

Render2DSystem::Render2DSystem(const Render2DSystem& other)
{
	m_vbo = 0;
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

	return !FAILED(result);
}

void Render2DSystem::CreateBatches(Scene* scene)
{
	std::vector<Sprite>* sprites = scene->GetComponents<Sprite>();
	for (Sprite& sprite : *sprites) {

		// Read in relevant data from sprite.
		Shader* shader = sprite.GetShader();
		Texture* texture = sprite.GetTexture();
		Transform& transform = scene->GetComponent<Transform>(sprite.GetEntityId());
		DirectX::XMFLOAT4 color = sprite.GetTint();
		DirectX::XMFLOAT4 uvBounds = sprite.GetUVBounds();

		// Try to find a batch.
		size_t batchIndex;
		for (batchIndex = 0; batchIndex < m_batches.size(); batchIndex++) {
			if (shader  == m_batches[batchIndex].shader  && 
				texture == m_batches[batchIndex].texture &&
				m_batches[batchIndex].vertices.size() < 6 * MAX_SPRITE_BATCH_SIZE) {
				break;
			}
		}

		// Could not find a viable batch, create a new one.
		if (batchIndex == m_batches.size()) {
			m_batches.emplace_back();
			m_batches[batchIndex].shader = shader;
			m_batches[batchIndex].texture = texture;
		}

		// Positions of the vertices of the unit quad centered at the origin.
		DirectX::XMVECTOR positions[4] = {
			{ -0.5f, -0.5f, 0.0f, 1.0f }, // Bottom-Left
			{ -0.5f,  0.5f, 0.0f, 1.0f }, // Top-Left
			{  0.5f,  0.5f, 0.0f, 1.0f }, // Top-Right
			{  0.5f, -0.5f, 0.0f, 1.0f }  // Bottom-Right
		};

		DirectX::XMMATRIX modelMatrix = transform.GetModelMatrix();

		// Transform the vertices by the sprite's model matrix.
		for (int i = 0; i < 4; ++i)
			positions[i] = DirectX::XMVector4Transform(positions[i], modelMatrix);

		// Generate the vertices for the sprite.
		Vertex2D bottomLeft;
		DirectX::XMStoreFloat3(&bottomLeft.position, positions[0]);
		bottomLeft.color = color;
		bottomLeft.texCoord = DirectX::XMFLOAT2(uvBounds.x, uvBounds.y);

		Vertex2D topLeft;
		DirectX::XMStoreFloat3(&topLeft.position, positions[1]);
		topLeft.color = color;
		topLeft.texCoord = DirectX::XMFLOAT2(uvBounds.x, uvBounds.w);

		Vertex2D topRight;
		DirectX::XMStoreFloat3(&topRight.position, positions[2]);
		topRight.color = color;
		topRight.texCoord = DirectX::XMFLOAT2(uvBounds.z, uvBounds.w);

		Vertex2D bottomRight;
		DirectX::XMStoreFloat3(&bottomRight.position, positions[3]);
		bottomRight.color = color;
		bottomRight.texCoord = DirectX::XMFLOAT2(uvBounds.z, uvBounds.y);

		// Add the vertices to the batch.
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

		// Copy the batched vertices to the dynamic vertex buffer.
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		result = deviceContext->Map(m_vbo, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		if (FAILED(result)) {
			return;
		}
		Vertex2D* dataPtr = (Vertex2D*)mappedResource.pData;
		memcpy(dataPtr, (void*)batch.vertices.data(), (sizeof(Vertex2D) * batch.vertices.size()));
		deviceContext->Unmap(m_vbo, 0);

		// Set the vertex buffer in the input assembler.
		deviceContext->IASetVertexBuffers(0, 1, &m_vbo, &stride, &offset);
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// Bind the shader.
		batch.shader->Bind(deviceContext, camera.GetViewMatrix(), camera.GetProjectionMatrix());

		// Set the textures and sampler states in the pixel shader.
		ID3D11SamplerState* sampler = batch.texture->GetSamplerState();
		ID3D11ShaderResourceView* texture = batch.texture->GetTexture2D();

		deviceContext->PSSetSamplers(0, 1, &sampler);
		deviceContext->PSSetShaderResources(0, 1, &texture);
		
		deviceContext->Draw(static_cast<unsigned int>(batch.vertices.size()), 0);
	}
}

void Render2DSystem::Update(Direct3D* direct3d, Scene* scene)
{
	std::vector<Camera2D>* cameras = scene->GetComponents<Camera2D>();

	direct3d->Clear(0.0f, 0.0f, 0.0f, 0.0f);

	CreateBatches(scene);
	for (Camera2D& camera : *cameras) {
		RenderBatches(direct3d, camera);
	}
	// Clear the batches.
	m_batches.clear();

	direct3d->Render();
}

void Render2DSystem::Shutdown() {
	if (m_vbo) {
		m_vbo->Release();
		m_vbo = 0;
	}
}

