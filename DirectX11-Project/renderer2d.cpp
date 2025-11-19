#include "renderer2d.h"
#include <array>

bool Renderer2D::Initialize(ID3D11Device* device)
{
	m_cameraView = DirectX::XMMatrixIdentity();
	m_cameraProjection = DirectX::XMMatrixIdentity();

	// Initialize indices.
	unsigned long* indices = new unsigned long[6 * MAX_SPRITE_BATCH_SIZE];
	for (unsigned long i = 0, v = 0; i < 6 * MAX_SPRITE_BATCH_SIZE; i+= 6, v+=4) {
		indices[i]     = v;
		indices[i + 1] = v + 1;
		indices[i + 2] = v + 2;
		indices[i + 3] = v;
		indices[i + 4] = v + 2;
		indices[i + 5] = v + 3;
	}

	HRESULT result;

	// Create the static index buffer desc allocating the max amount of vertices.
	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * 6 * MAX_SPRITE_BATCH_SIZE;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_ibo);

	delete[] indices;

	if (FAILED(result)) {
		return false;
	}

	// Create the dynamic vertex buffer desc allocating the max amount of vertices.
	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(Engine::Vertex2D) * 4 * MAX_SPRITE_BATCH_SIZE;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Create the dynamic vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, 0, &m_vbo);

	return !FAILED(result);
}

Renderer2D::~Renderer2D()
{
	if (m_vbo) {
		m_vbo->Release();
		m_vbo = 0;
	}
	if (m_ibo) {
		m_ibo->Release();
		m_ibo = 0;
	}
}

void Renderer2D::BeginScene(Camera2D& camera)
{
	m_cameraView = camera.GetViewMatrix();
	m_cameraProjection = camera.GetProjectionMatrix();

	m_lastBatchIndex = 0;

	// Clear the batches.
	for (SpriteBatch& batch : m_batches) {
		batch.shader = 0;
		batch.texture = 0;
		batch.vertices.clear();
	}
}

void const Renderer2D::SubmitSprite(const SpriteData& spriteData, const DirectX::XMMATRIX& modelMatrix)
{
	// Try to validate spriteData
	if (!spriteData.shader || !spriteData.texture) {
		return;
	}

	// Try to find a batch.
	size_t batchIndex;

	if ((m_lastBatchIndex < m_batches.size()) &&
		(m_batches[m_lastBatchIndex].vertices.size() < 4 * MAX_SPRITE_BATCH_SIZE) &&
		(spriteData.shader == m_batches[m_lastBatchIndex].shader || m_batches[m_lastBatchIndex].shader == 0) &&
		(spriteData.texture == m_batches[m_lastBatchIndex].texture || m_batches[m_lastBatchIndex].texture == 0)) {
		batchIndex = m_lastBatchIndex;
	}
	else {
		for (batchIndex = 0; batchIndex < m_batches.size(); batchIndex++) {
			if ((m_batches[batchIndex].vertices.size() < 4 * MAX_SPRITE_BATCH_SIZE) &&
				(spriteData.shader == m_batches[batchIndex].shader || m_batches[batchIndex].shader == 0) &&
				(spriteData.texture == m_batches[batchIndex].texture || m_batches[batchIndex].texture == 0)) {
				break;
			}
		}
	}

	// Could not find a viable batch, create a new one.
	if (batchIndex == m_batches.size()) {
		m_batches.emplace_back();
	}

	m_lastBatchIndex = batchIndex;

	// Always overwrite batch parameters.
	m_batches[batchIndex].shader = spriteData.shader;
	m_batches[batchIndex].texture = spriteData.texture;

	// Positions of the vertices of the unit quad centered at the origin.
	DirectX::XMVECTOR positions[4] = {
		{ -0.5f, -0.5f, 0.0f, 1.0f }, // Bottom-Left
		{ -0.5f,  0.5f, 0.0f, 1.0f }, // Top-Left
		{  0.5f,  0.5f, 0.0f, 1.0f }, // Top-Right
		{  0.5f, -0.5f, 0.0f, 1.0f }  // Bottom-Right
	};

	// Transform the vertices by the sprite's model matrix.
	for (int i = 0; i < 4; ++i)
		positions[i] = DirectX::XMVector4Transform(positions[i], modelMatrix);

	float textureWidth = static_cast<float>(spriteData.texture->GetWidth());
	float textureHeight = static_cast<float>(spriteData.texture->GetHeight());

	// Caclulate UV bounds.
	// Offset by half a texel to sample from pixel center.
	float halfTexelU = 0.5f / textureWidth;
	float halfTexelV = 0.5f / textureHeight;

	float sourceWidth = spriteData.sourceRect.z == 0 ? spriteData.texture->GetWidth() : spriteData.sourceRect.z;
	float sourceHeight = spriteData.sourceRect.w == 0 ? spriteData.texture->GetHeight() : spriteData.sourceRect.w;

	float u1 = (spriteData.sourceRect.x + halfTexelU) / textureWidth;
	float v1 = (spriteData.sourceRect.y + halfTexelV) / textureHeight;

	float u2 = (spriteData.sourceRect.x + sourceWidth + halfTexelU) / textureWidth;
	float v2 = (spriteData.sourceRect.y + sourceHeight + halfTexelV) / textureHeight;

	size_t firstVertex = m_batches[batchIndex].vertices.size();

	m_batches[batchIndex].vertices.emplace_back(Engine::Vertex2D{ {0.0f, 0.0f, 0.0f}, {u1, v1}, {spriteData.tint } });
	m_batches[batchIndex].vertices.emplace_back(Engine::Vertex2D{ {0.0f, 0.0f, 0.0f}, {u1, v2}, {spriteData.tint } });
	m_batches[batchIndex].vertices.emplace_back(Engine::Vertex2D{ {0.0f, 0.0f, 0.0f}, {u2, v2}, {spriteData.tint } });
	m_batches[batchIndex].vertices.emplace_back(Engine::Vertex2D{ {0.0f, 0.0f, 0.0f}, {u2, v1}, {spriteData.tint } });

	for (size_t i = 0; i < 4; i++)
	{
		DirectX::XMStoreFloat3(&m_batches[batchIndex].vertices[firstVertex + i].position, positions[i]);
	}
}

void const Renderer2D::SubmitSprite(const SpriteData& spriteData, float centerX, float centerY, float width, float height)
{
	if (!spriteData.shader || !spriteData.texture) {
		return;
	}

	DirectX::XMMATRIX modelMatrix =
		DirectX::XMMatrixScaling(width, height, 1.0f) *
		DirectX::XMMatrixRotationZ(0.0f) *
		DirectX::XMMatrixTranslation(centerX, centerX, 0.0f);

	SubmitSprite(spriteData, modelMatrix);
}

void const Renderer2D::SubmitSprite(const SpriteData& spriteData, float centerX, float centerY, float width, float height, float rotationDeg)
{
	if (!spriteData.shader || !spriteData.texture) {
		return;
	}

	DirectX::XMMATRIX modelMatrix =
		DirectX::XMMatrixScaling(width, height, 1.0f) *
		DirectX::XMMatrixRotationZ(rotationDeg) *
		DirectX::XMMatrixTranslation(centerX, centerX, 0.0f);

	SubmitSprite(spriteData, modelMatrix);
}

void const Renderer2D::SubmitSprite(const SpriteData& spriteData, float centerX, float centerY, float width, float height, DirectX::XMVECTOR rotationQuaternion)
{
	if (!spriteData.shader || !spriteData.texture) {
		return;
	}

	DirectX::XMMATRIX modelMatrix =
		DirectX::XMMatrixScaling(width, height, 1.0f) *
		DirectX::XMMatrixRotationQuaternion(rotationQuaternion) *
		DirectX::XMMatrixTranslation(centerX, centerX, 0.0f);

	SubmitSprite(spriteData, modelMatrix);
}

void const Renderer2D::SubmitText(const TextData& textData, const DirectX::XMMATRIX& modelMatrix)
{
	if (!textData.shader || !textData.texture) {
		return;
	}

	SpriteData spriteData;
	spriteData.shader = textData.shader;
	spriteData.texture = textData.texture;
	spriteData.sourceRect.z = static_cast<float>(textData.characterDimensions.x);
	spriteData.sourceRect.w = static_cast<float>(textData.characterDimensions.y);
	spriteData.tint = textData.tint;

	DirectX::XMMATRIX characterModelMatrix;
	

	size_t textLength = textData.text.length();

	float column = 0.0f;
	float row = 0.0f;
	

	for (size_t i = 0; i < textLength; i++) {
		if (textData.text[i] == '\n') {
			column = 0.0f;
			row -= 1.0f;
		}
		else {
			column += 1.0f;
		}

		characterModelMatrix = DirectX::XMMatrixTranslation(column, row, 0.0f) * modelMatrix;
		spriteData.sourceRect.x = static_cast<float>((textData.text[i] - ' ') * textData.characterDimensions.x);
		SubmitSprite(spriteData, characterModelMatrix);
	}
}

void Renderer2D::EndScene(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	HRESULT result;

	unsigned int stride = sizeof(Engine::Vertex2D);
	unsigned int offset = 0;

	for (SpriteBatch& batch : m_batches) {

		if (!batch.shader) {
			continue;
		}

		// Copy the batched vertices to the dynamic vertex buffer.
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		result = deviceContext->Map(m_vbo, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		if (FAILED(result)) {
			return;
		}
		Engine::Vertex2D* dataPtr = (Engine::Vertex2D*)mappedResource.pData;
		memcpy(dataPtr, (void*)batch.vertices.data(), (sizeof(Engine::Vertex2D) * batch.vertices.size()));
		deviceContext->Unmap(m_vbo, 0);

		// Set the index and vertex buffer in the input assembler.
		deviceContext->IASetIndexBuffer(m_ibo, DXGI_FORMAT_R32_UINT, 0);
		deviceContext->IASetVertexBuffers(0, 1, &m_vbo, &stride, &offset);
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// Bind the shader.
		batch.shader->Bind(deviceContext, m_cameraView, m_cameraProjection);

		// Upload texture to device.
		batch.texture->Upload(device, deviceContext);

		// Bind the texture.
		batch.texture->Bind(deviceContext);

		// Calculate the index count.
		size_t indexCount = static_cast<unsigned int>(static_cast<float>(batch.vertices.size()) * 1.5f);

		// Draw the batch.
		deviceContext->DrawIndexed(indexCount, 0, 0);
	}
}