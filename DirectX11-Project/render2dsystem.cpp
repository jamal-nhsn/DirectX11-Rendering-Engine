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
		ID3D11SamplerState* sampler = sprite.GetTexture()->GetSamplerState();
		ID3D11ShaderResourceView* texture = sprite.GetTexture()->GetTexture2D();
		Transform& transform = scene->GetComponent<Transform>(sprite.GetEntityId());

		// Try to find a batch.
		size_t batchIndex;
		size_t samplerIndex;
		size_t textureIndex;
		for (batchIndex = 0; batchIndex < m_batches.size(); batchIndex++) {

			// Keep looking if shader mismatch or the batch is full.
			if (shader != m_batches[batchIndex].shader || m_batches[batchIndex].vertices.size() >= 6 * MAX_SPRITE_BATCH_SIZE) {
				continue;
			}

			// Try to find a matching sampler in the batch.
			for (samplerIndex = 0; samplerIndex < m_batches[batchIndex].samplers.size(); samplerIndex++) {
				if (m_batches[batchIndex].samplers[samplerIndex] == sampler) {
					break;
				}
			}

			// Try to find a matching a texture in the batch.
			for (textureIndex = 0; textureIndex < m_batches[batchIndex].textures.size(); textureIndex++) {
				if (m_batches[batchIndex].textures[textureIndex] == texture) {
					break;
				}
			}

			// Keep looking if no matching sampler and samplers are full.
			if (samplerIndex == m_batches[batchIndex].samplers.size() && m_batches[batchIndex].samplers.size() >= D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT) {
				continue;
			}

			// Keep looking if no matching texture and textures are full.
			if (textureIndex == m_batches[batchIndex].textures.size() && m_batches[batchIndex].textures.size() >= D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT) {
				continue;
			}

			// Register a new sampler if no matching sampler was found and batch has space for one.
			if (samplerIndex == m_batches[batchIndex].samplers.size()) {
				m_batches[batchIndex].samplers.emplace_back(sampler);
			}

			// Register a new texture if no matching texture was found and batch has space for one.
			if (textureIndex == m_batches[batchIndex].textures.size()) {
				m_batches[batchIndex].textures.emplace_back(texture);
			}

			// Found a viable batch, stop looking.
			break;
		}

		// Could not find a viable batch, create a new one.
		if (batchIndex == m_batches.size()) {
			m_batches.emplace_back();
			m_batches[batchIndex].shader = shader;
			m_batches[batchIndex].samplers.emplace_back(sampler);
			m_batches[batchIndex].textures.emplace_back(texture);
			samplerIndex = 0;
			textureIndex = 0;
		}

		// Positions of the vertices of the unit quad centered at the origin.
		DirectX::XMVECTOR positions[4] = {
			{ -0.5f, -0.5f, 0.0f, 1.0f }, // Bottom-Left
			{ -0.5f,  0.5f, 0.0f, 1.0f }, // Top-Left
			{  0.5f,  0.5f, 0.0f, 1.0f }, // Top-Right
			{  0.5f, -0.5f, 0.0f, 1.0f }  // Bottom-Right
		};

		// Transform the vertices by the sprite's model matrix.
		for (int i = 0; i < 4; ++i)
			positions[i] = DirectX::XMVector4Transform(positions[i], transform.GetModelMatrix());

		// Read in additional sprite data.
		DirectX::XMFLOAT4 color = sprite.GetTint();
		DirectX::XMFLOAT4 uvBounds = sprite.GetUVBounds();

		// Generate the vertices for the sprite.
		Vertex2D bottomLeft;
		DirectX::XMStoreFloat3(&bottomLeft.position, positions[0]);
		bottomLeft.color = color;
		bottomLeft.texCoord = DirectX::XMFLOAT2(uvBounds.x, uvBounds.y);
		bottomLeft.samplerIndex = static_cast<float>(samplerIndex);
		bottomLeft.textureIndex = static_cast<float>(textureIndex);

		Vertex2D topLeft;
		DirectX::XMStoreFloat3(&topLeft.position, positions[1]);
		topLeft.color = color;
		topLeft.texCoord = DirectX::XMFLOAT2(uvBounds.x, uvBounds.w);
		topLeft.samplerIndex = static_cast<float>(samplerIndex);
		topLeft.textureIndex = static_cast<float>(textureIndex);

		Vertex2D topRight;
		DirectX::XMStoreFloat3(&topRight.position, positions[2]);
		topRight.color = color;
		topRight.texCoord = DirectX::XMFLOAT2(uvBounds.z, uvBounds.w);
		topRight.samplerIndex = static_cast<float>(samplerIndex);
		topRight.textureIndex = static_cast<float>(textureIndex);

		Vertex2D bottomRight;
		DirectX::XMStoreFloat3(&bottomRight.position, positions[3]);
		bottomRight.color = color;
		bottomRight.texCoord = DirectX::XMFLOAT2(uvBounds.z, uvBounds.y);
		bottomRight.samplerIndex = static_cast<float>(samplerIndex);
		bottomRight.textureIndex = static_cast<float>(textureIndex);

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

		//batch.shader->Bind()

		// Set the textures and sampler states in the pixel shader.
		deviceContext->PSSetShaderResources(0, static_cast<unsigned int>(batch.textures.size()), batch.textures.data());
		deviceContext->PSSetSamplers(0, static_cast<unsigned int>(batch.samplers.size()), batch.samplers.data());

		deviceContext->Draw(static_cast<unsigned int>(batch.vertices.size()), 0);
	}
}

void Render2DSystem::Update(Direct3D* direct3d, Scene* scene)
{
	std::vector<Camera2D>* cameras = scene->GetComponents<Camera2D>();

	CreateBatches(scene);
	for (Camera2D& camera : *cameras) {
		RenderBatches(direct3d, camera);
	}
	// Clear the batches.
	m_batches.clear();
}

void Render2DSystem::Shutdown() {
	if (m_vbo) {
		m_vbo->Release();
		m_vbo = 0;
	}
}

