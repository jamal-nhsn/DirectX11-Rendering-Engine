#include "renderer2d.h"

bool Renderer2D::Initialize(ID3D11Device* device)
{
	m_cameraView = DirectX::XMMatrixIdentity();
	m_cameraProjection = DirectX::XMMatrixIdentity();

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

Renderer2D::~Renderer2D()
{
	if (m_vbo) {
		m_vbo->Release();
		m_vbo = 0;
	}
}

void Renderer2D::BeginScene(Camera2D& camera)
{
	m_cameraView = camera.GetViewMatrix();
	m_cameraProjection = camera.GetProjectionMatrix();

	// Clear the batches.
	for (SpriteBatch& batch : m_batches) {
		batch.shader = 0;
		batch.texture = 0;
		batch.vertices.clear();
	}
}

void const Renderer2D::SubmitSprite(
	Shader* shader, Texture* texture,
	DirectX::XMMATRIX& modelMatrix,
	int sourceX, int sourceY,
	int sourceWidth, int sourceHeight,
	DirectX::XMFLOAT4& tint
)
{
	// Try to find a batch.
	size_t batchIndex;
	for (batchIndex = 0; batchIndex < m_batches.size(); batchIndex++) {
		if ((shader == m_batches[batchIndex].shader || m_batches[batchIndex].shader == 0) &&
			(texture == m_batches[batchIndex].texture || m_batches[batchIndex].texture == 0) &&
			m_batches[batchIndex].vertices.size() < 6 * MAX_SPRITE_BATCH_SIZE) {
			break;
		}
	}

	// Could not find a viable batch, create a new one.
	if (batchIndex == m_batches.size()) {
		m_batches.emplace_back();
	}

	// Always overwrite batch parameters.
	m_batches[batchIndex].shader = shader;
	m_batches[batchIndex].texture = texture;

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

	float textureWidth = static_cast<float>(texture->GetWidth());
	float textureHeight = static_cast<float>(texture->GetHeight());

	// Caclulate UV bounds.
	// Offset by half a texel to sample from pixel center.
	float halfTexelU = 0.5f / textureWidth;
	float halfTexelV = 0.5f / textureHeight;

	float u1 = (static_cast<float>(sourceX) + halfTexelU) / textureWidth;
	float v1 = (static_cast<float>(sourceY) + halfTexelV) / textureHeight;

	float u2 = (static_cast<float>(sourceX + sourceWidth) + halfTexelU) / textureWidth;
	float v2 = (static_cast<float>(sourceY + sourceHeight) + halfTexelV) / textureHeight;

	// Generate the vertices for the sprite.
	Vertex2D bottomLeft;
	DirectX::XMStoreFloat3(&bottomLeft.position, positions[0]);
	bottomLeft.color = tint;
	bottomLeft.texCoord = DirectX::XMFLOAT2(u1, v1);

	Vertex2D topLeft;
	DirectX::XMStoreFloat3(&topLeft.position, positions[1]);
	topLeft.color = tint;
	topLeft.texCoord = DirectX::XMFLOAT2(u1, v2);

	Vertex2D topRight;
	DirectX::XMStoreFloat3(&topRight.position, positions[2]);
	topRight.color = tint;
	topRight.texCoord = DirectX::XMFLOAT2(u2, v2);

	Vertex2D bottomRight;
	DirectX::XMStoreFloat3(&bottomRight.position, positions[3]);
	bottomRight.color = tint;
	bottomRight.texCoord = DirectX::XMFLOAT2(u2, v1);

	// Add the vertices to the batch.
	m_batches[batchIndex].vertices.emplace_back(bottomLeft);
	m_batches[batchIndex].vertices.emplace_back(topLeft);
	m_batches[batchIndex].vertices.emplace_back(topRight);
	m_batches[batchIndex].vertices.emplace_back(bottomLeft);
	m_batches[batchIndex].vertices.emplace_back(topRight);
	m_batches[batchIndex].vertices.emplace_back(bottomRight);
}

void Renderer2D::EndScene(ID3D11DeviceContext* deviceContext)
{
	HRESULT result;

	unsigned int stride = sizeof(Vertex2D);
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
		Vertex2D* dataPtr = (Vertex2D*)mappedResource.pData;
		memcpy(dataPtr, (void*)batch.vertices.data(), (sizeof(Vertex2D) * batch.vertices.size()));
		deviceContext->Unmap(m_vbo, 0);

		// Set the vertex buffer in the input assembler.
		deviceContext->IASetVertexBuffers(0, 1, &m_vbo, &stride, &offset);
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// Bind the shader.
		batch.shader->Bind(deviceContext, m_cameraView, m_cameraProjection);

		// Set the textures and sampler states in the pixel shader.
		ID3D11SamplerState* sampler = batch.texture->GetSamplerState();
		ID3D11ShaderResourceView* texture = batch.texture->GetTexture2D();

		deviceContext->PSSetSamplers(0, 1, &sampler);
		deviceContext->PSSetShaderResources(0, 1, &texture);

		// Draw the batch.
		deviceContext->Draw(static_cast<unsigned int>(batch.vertices.size()), 0);
	}
}

void const Renderer2D::SubmitSprite(
	Shader* shader, Texture* texture,
	float destX, float destY,
	float destWidth, float destHeight
)
{
	DirectX::XMMATRIX modelMatrix =
		DirectX::XMMatrixScaling(destWidth, destHeight, 1.0f) *
		DirectX::XMMatrixTranslation(destX, destY, 0.0f);
	
	DirectX::XMFLOAT4 tint(1.0f, 1.0f, 1.0f, 1.0f);
	
	SubmitSprite(
		shader, texture,
		modelMatrix,
		0, 0,
		texture->GetWidth(), texture->GetHeight(),
		tint
	);
}
void const Renderer2D::SubmitSprite(
	Shader* shader, Texture* texture,
	float destX, float destY,
	float destWidth, float destHeight,
	float rotation
)
{
	DirectX::XMMATRIX modelMatrix =
		DirectX::XMMatrixScaling(destWidth, destHeight, 1.0f) *
		DirectX::XMMatrixRotationQuaternion(DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), DirectX::XMConvertToRadians(rotation))) *
		DirectX::XMMatrixTranslation(destX, destY, 0.0f);

	DirectX::XMFLOAT4 tint(1.0f, 1.0f, 1.0f, 1.0f);

	SubmitSprite(
		shader, texture,
		modelMatrix,
		0, 0,
		texture->GetWidth(), texture->GetHeight(),
		tint
	);
}
void const Renderer2D::SubmitSprite(
	Shader* shader, Texture* texture,
	float destX, float destY,
	float destWidth, float destHeight,
	DirectX::XMVECTOR rotation
)
{
	DirectX::XMMATRIX modelMatrix =
		DirectX::XMMatrixScaling(destWidth, destHeight, 1.0f) *
		DirectX::XMMatrixRotationQuaternion(rotation) *
		DirectX::XMMatrixTranslation(destX, destY, 0.0f);

	DirectX::XMFLOAT4 tint(1.0f, 1.0f, 1.0f, 1.0f);

	SubmitSprite(
		shader, texture,
		modelMatrix,
		0, 0,
		texture->GetWidth(), texture->GetHeight(),
		tint
	);
}
void const Renderer2D::SubmitSprite(
	Shader* shader, Texture* texture,
	DirectX::XMMATRIX& modelMatrix
)
{
	DirectX::XMFLOAT4 tint(1.0f, 1.0f, 1.0f, 1.0f);

	SubmitSprite(
		shader, texture,
		modelMatrix,
		0, 0,
		texture->GetWidth(), texture->GetHeight(),
		tint
	);
}
void const Renderer2D::SubmitSprite(
	Shader* shader, Texture* texture,
	float destX, float destY,
	float destWidth, float destHeight,
	int sourceX, int sourceY,
	int sourceWidth, int sourceHeight
)
{
	DirectX::XMMATRIX modelMatrix =
		DirectX::XMMatrixScaling(destWidth, destHeight, 1.0f) *
		DirectX::XMMatrixTranslation(destX, destY, 0.0f);

	DirectX::XMFLOAT4 tint(1.0f, 1.0f, 1.0f, 1.0f);

	SubmitSprite(
		shader, texture,
		modelMatrix,
		sourceX, sourceY,
		sourceWidth, sourceHeight,
		tint
	);
}
void const Renderer2D::SubmitSprite(
	Shader* shader, Texture* texture,
	float destX, float destY,
	float destWidth, float destHeight,
	float rotation,
	int sourceX, int sourceY,
	int sourceWidth, int sourceHeight
)
{
	DirectX::XMMATRIX modelMatrix =
		DirectX::XMMatrixScaling(destWidth, destHeight, 1.0f) *
		DirectX::XMMatrixRotationQuaternion(DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), DirectX::XMConvertToRadians(rotation))) *
		DirectX::XMMatrixTranslation(destX, destY, 0.0f);

	DirectX::XMFLOAT4 tint(1.0f, 1.0f, 1.0f, 1.0f);

	SubmitSprite(
		shader, texture,
		modelMatrix,
		sourceX, sourceY,
		sourceWidth, sourceHeight,
		tint
	);
}
void const Renderer2D::SubmitSprite(
	Shader* shader, Texture* texture,
	float destX, float destY,
	float destWidth, float destHeight,
	DirectX::XMVECTOR rotation,
	int sourceX, int sourceY,
	int sourceWidth, int sourceHeight
)
{
	DirectX::XMMATRIX modelMatrix =
		DirectX::XMMatrixScaling(destWidth, destHeight, 1.0f) *
		DirectX::XMMatrixRotationQuaternion(rotation) *
		DirectX::XMMatrixTranslation(destX, destY, 0.0f);

	DirectX::XMFLOAT4 tint(1.0f, 1.0f, 1.0f, 1.0f);

	SubmitSprite(
		shader, texture,
		modelMatrix,
		sourceX, sourceY,
		sourceWidth, sourceHeight,
		tint
	);
}
void const Renderer2D::SubmitSprite(
	Shader* shader, Texture* texture,
	DirectX::XMMATRIX& modelMatrix,
	int sourceX, int sourceY,
	int sourceWidth, int sourceHeight
)
{
	DirectX::XMFLOAT4 tint(1.0f, 1.0f, 1.0f, 1.0f);

	SubmitSprite(
		shader, texture,
		modelMatrix,
		sourceX, sourceY,
		sourceWidth, sourceHeight,
		tint
	);
}
void const Renderer2D::SubmitSprite(
	Shader* shader, Texture* texture,
	float destX, float destY,
	float destWidth, float destHeight,
	DirectX::XMFLOAT4& tint
)
{
	DirectX::XMMATRIX modelMatrix =
		DirectX::XMMatrixScaling(destWidth, destHeight, 1.0f) *
		DirectX::XMMatrixTranslation(destX, destY, 0.0f);

	SubmitSprite(
		shader, texture,
		modelMatrix,
		0, 0,
		texture->GetWidth(), texture->GetHeight(),
		tint
	);
}
void const Renderer2D::SubmitSprite(
	Shader* shader, Texture* texture,
	float destX, float destY,
	float destWidth, float destHeight,
	float rotation,
	DirectX::XMFLOAT4& tint
)
{
	DirectX::XMMATRIX modelMatrix =
		DirectX::XMMatrixScaling(destWidth, destHeight, 1.0f) *
		DirectX::XMMatrixRotationQuaternion(DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), DirectX::XMConvertToRadians(rotation))) *
		DirectX::XMMatrixTranslation(destX, destY, 0.0f);

	SubmitSprite(
		shader, texture,
		modelMatrix,
		0, 0,
		texture->GetWidth(), texture->GetHeight(),
		tint
	);
}
void const Renderer2D::SubmitSprite(
	Shader* shader, Texture* texture,
	float destX, float destY,
	float destWidth, float destHeight,
	DirectX::XMVECTOR rotation,
	DirectX::XMFLOAT4& tint
)
{
	DirectX::XMMATRIX modelMatrix =
		DirectX::XMMatrixScaling(destWidth, destHeight, 1.0f) *
		DirectX::XMMatrixRotationQuaternion(rotation) *
		DirectX::XMMatrixTranslation(destX, destY, 0.0f);

	SubmitSprite(
		shader, texture,
		modelMatrix,
		0, 0,
		texture->GetWidth(), texture->GetHeight(),
		tint
	);
}
void const Renderer2D::SubmitSprite(
	Shader* shader, Texture* texture,
	DirectX::XMMATRIX& modelMatrix,
	DirectX::XMFLOAT4& tint
)
{
	SubmitSprite(
		shader, texture,
		modelMatrix,
		0, 0,
		texture->GetWidth(), texture->GetHeight(),
		tint
	);
}
void const Renderer2D::SubmitSprite(
	Shader* shader, Texture* texture,
	float destX, float destY,
	float destWidth, float destHeight,
	int sourceX, int sourceY,
	int sourceWidth, int sourceHeight,
	DirectX::XMFLOAT4& tint
)
{
	DirectX::XMMATRIX modelMatrix =
		DirectX::XMMatrixScaling(destWidth, destHeight, 1.0f) *
		DirectX::XMMatrixTranslation(destX, destY, 0.0f);

	SubmitSprite(
		shader, texture,
		modelMatrix,
		sourceX, sourceY,
		sourceWidth, sourceHeight,
		tint
	);
}
void const Renderer2D::SubmitSprite(
	Shader* shader, Texture* texture,
	float destX, float destY,
	float destWidth, float destHeight,
	float rotation,
	int sourceX, int sourceY,
	int sourceWidth, int sourceHeight,
	DirectX::XMFLOAT4& tint
)
{
	DirectX::XMMATRIX modelMatrix =
		DirectX::XMMatrixScaling(destWidth, destHeight, 1.0f) *
		DirectX::XMMatrixRotationQuaternion(DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), DirectX::XMConvertToRadians(rotation))) *
		DirectX::XMMatrixTranslation(destX, destY, 0.0f);

	SubmitSprite(
		shader, texture,
		modelMatrix,
		sourceX, sourceY,
		sourceWidth, sourceHeight,
		tint
	);
}
void const Renderer2D::SubmitSprite(
	Shader* shader, Texture* texture,
	float destX, float destY,
	float destWidth, float destHeight,
	DirectX::XMVECTOR rotation,
	int sourceX, int sourceY,
	int sourceWidth, int sourceHeight,
	DirectX::XMFLOAT4& tint
)
{
	DirectX::XMMATRIX modelMatrix =
		DirectX::XMMatrixScaling(destWidth, destHeight, 1.0f) *
		DirectX::XMMatrixRotationQuaternion(rotation) *
		DirectX::XMMatrixTranslation(destX, destY, 0.0f);

	SubmitSprite(
		shader, texture,
		modelMatrix,
		sourceX, sourceY,
		sourceWidth, sourceHeight,
		tint
	);
}