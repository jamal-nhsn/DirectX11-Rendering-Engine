#include "textureshader.h"
#include "scene.h"

TextureShader::TextureShader()
{
	wcscpy_s(m_vertexShaderSource, 128, L"../DirectX11-Project/texture.vs");
	wcscpy_s(m_pixelShaderSource, 128, L"../DirectX11-Project/texture.ps");

	m_vertexShader = 0;
	m_pixelShader = 0;
	m_layout = 0;
	m_sampleState = 0;
	m_vertexConstantBuffer = 0;
}

TextureShader::TextureShader(const TextureShader& other)
{
	m_vertexShader = other.m_vertexShader;
	m_pixelShader = other.m_pixelShader;
	m_vertexConstantBuffer = other.m_vertexConstantBuffer;
	m_sampleState = other.m_sampleState;
	m_layout = other.m_layout;
}

TextureShader::~TextureShader()
{
}

bool TextureShader::Bind(ID3D11DeviceContext* deviceContext, Camera& camera, Model& model, Transform& modelTransform, Light& light, Transform& lightTransform)
{
	bool success;
	VertexConstantBuffer vertexConstantBuffer;
	ID3D11ShaderResourceView* texture;

	deviceContext->IASetInputLayout(m_layout);
	deviceContext->VSSetShader(m_vertexShader, NULL, 0);
	deviceContext->PSSetShader(m_pixelShader, NULL, 0);

	// Transpose the matrices to prepare them for the shader.
	vertexConstantBuffer.model = DirectX::XMMatrixTranspose(modelTransform.GetModelMatrix());
	vertexConstantBuffer.view = DirectX::XMMatrixTranspose(camera.GetViewMatrix());
	vertexConstantBuffer.projection = DirectX::XMMatrixTranspose(camera.GetProjectionMatrix());

	texture = model.GetTexture()->GetTexture2D();
	
	success = SetShaderParameters(deviceContext, vertexConstantBuffer, texture);
	if (!success) {
		return success;
	}

	// Set the sampler state in the pixel shader.
	deviceContext->PSSetSamplers(0, 1, &m_sampleState);

	return success;
}

bool TextureShader::IsLit()
{
	return false;
}

bool TextureShader::SetShaderParameters(ID3D11DeviceContext* deviceContext, VertexConstantBuffer vertexConstantBuffer, ID3D11ShaderResourceView* texture)
{
	HRESULT result;

	// Lock the constant buffer so it can be written to.
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	result = deviceContext->Map(m_vertexConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result)) {
		return false;
	}

	// Get a pointer to the data in the constant buffer.
	VertexConstantBuffer* vertexDataPtr;
	vertexDataPtr = (VertexConstantBuffer*)mappedResource.pData;

	// Copy the matrices into the constant buffer.
	vertexDataPtr->model = vertexConstantBuffer.model;
	vertexDataPtr->view = vertexConstantBuffer.view;
	vertexDataPtr->projection = vertexConstantBuffer.projection;

	// Unlock the constant buffer.
	deviceContext->Unmap(m_vertexConstantBuffer, 0);

	// Set the position of the constant buffer in the vertex shader.
	unsigned int bufferNumber = 0;

	// Set the constant buffer in the vertex shader with the updated values.
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_vertexConstantBuffer);

	// Finally, set shader texture resource in the pixel shader.
	deviceContext->PSSetShaderResources(0, 1, &texture);

	return true;
}

bool TextureShader::InitializeLayout(ID3D11Device* device, ID3D10Blob* vertexShaderBuffer, ID3D10Blob* pixelShaderBuffer)
{
	HRESULT result;
	unsigned int numElements;

	D3D11_INPUT_ELEMENT_DESC* polygonLayout = CreateLayout(
		true,  // Use position.
		false, // Use normal.
		true,  // Use texcoord.
		false, // Use tangent.
		false, // Use color.
		numElements
	);

	if (!polygonLayout) {
		return false;
	}

	// Create the vertex input layout.
	result = device->CreateInputLayout(
		polygonLayout,
		numElements,
		vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(),
		&m_layout
	);

	delete[] polygonLayout;

	return !FAILED(result);
}

bool TextureShader::InitializeSamplerDesc(ID3D11Device* device)
{
	HRESULT result;
	D3D11_SAMPLER_DESC samplerDesc;

	// Create a texture sampler state description.
	samplerDesc.Filter         = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU       = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV       = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW       = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias     = 0.0f;
	samplerDesc.MaxAnisotropy  = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD         = 0;
	samplerDesc.MaxLOD         = D3D11_FLOAT32_MAX;

	// Create the texture sampler state.
	result = device->CreateSamplerState(&samplerDesc, &m_sampleState);
	return !FAILED(result);
}

bool TextureShader::InitializeConstants(ID3D11Device* device)
{
	HRESULT result;
	D3D11_BUFFER_DESC constantBufferDesc;

	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	constantBufferDesc.ByteWidth = sizeof(VertexConstantBuffer);
	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constantBufferDesc.MiscFlags = 0;
	constantBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = device->CreateBuffer(&constantBufferDesc, NULL, &m_vertexConstantBuffer);
	return !FAILED(result);
}