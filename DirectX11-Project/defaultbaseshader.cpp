#include "defaultbaseshader.h"
#include "scene.h"

DefaultBaseShader::DefaultBaseShader()
{
	wcscpy_s(m_vertexShaderSource, 128, L"../DirectX11-Project/defaultbase.vs");
	wcscpy_s(m_pixelShaderSource, 128, L"../DirectX11-Project/defaultbase.ps");

	m_vertexShader = 0;
	m_pixelShader = 0;
	m_layout = 0;
	m_sampleState = 0;
	m_vertexConstantBuffer = 0;
}

DefaultBaseShader::DefaultBaseShader(const DefaultBaseShader& other)
{
	m_vertexShader = other.m_vertexShader;
	m_pixelShader = other.m_pixelShader;
	m_vertexConstantBuffer = other.m_vertexConstantBuffer;
	m_sampleState = other.m_sampleState;
	m_layout = other.m_layout;
}

DefaultBaseShader::~DefaultBaseShader()
{
}

bool DefaultBaseShader::Bind(ID3D11DeviceContext* deviceContext, Camera& camera, Model& model, Transform& modelTransform, DirectX::XMFLOAT4 ambientLight)
{
	bool success;
	VertexConstantBuffer vertexConstantBuffer;
	PixelConstantBuffer pixelConstantBuffer;
	ID3D11ShaderResourceView* texture;

	deviceContext->IASetInputLayout(m_layout);
	deviceContext->VSSetShader(m_vertexShader, NULL, 0);
	deviceContext->PSSetShader(m_pixelShader, NULL, 0);

	// Transpose the matrices to prepare them for the shader.
	vertexConstantBuffer.model = DirectX::XMMatrixTranspose(modelTransform.GetModelMatrix());
	vertexConstantBuffer.view = DirectX::XMMatrixTranspose(camera.GetViewMatrix());
	vertexConstantBuffer.projection = DirectX::XMMatrixTranspose(camera.GetProjectionMatrix());

	// Pass the ambient light to the pixel shader.
	pixelConstantBuffer.ambientLight = ambientLight;

	texture = model.GetTexture()->GetTexture2D();

	success = SetShaderParameters(deviceContext, vertexConstantBuffer, pixelConstantBuffer, texture);
	if (!success) {
		return success;
	}

	// Finally, set the shader states.
	SetShaderStates(deviceContext);

	return success;
}

bool DefaultBaseShader::SetShaderParameters(ID3D11DeviceContext* deviceContext, VertexConstantBuffer vertexConstantBuffer, PixelConstantBuffer pixelConstantBuffer, ID3D11ShaderResourceView* texture)
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

	// Lock the constant buffer again for the pixel constant buffer.
	result = deviceContext->Map(m_pixelConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result)) {
		return false;
	}

	// Get a pointer to the data in the constant buffer.
	PixelConstantBuffer* pixelDataPtr;
	pixelDataPtr = (PixelConstantBuffer*)mappedResource.pData;

	// Copy the ambient light data into the constant buffer.
	pixelDataPtr->ambientLight = pixelConstantBuffer.ambientLight;

	// Set the constant buffer in the pixel shader with the updated values.
	deviceContext->PSSetConstantBuffers(bufferNumber, 1, &m_pixelConstantBuffer);

	// Unlock the constant buffer.
	deviceContext->Unmap(m_pixelConstantBuffer, 0);

	// Finally, set shader texture resource in the pixel shader.
	deviceContext->PSSetShaderResources(0, 1, &texture);

	return true;
}

bool DefaultBaseShader::InitializeLayout(ID3D11Device* device, ID3D10Blob* vertexShaderBuffer, ID3D10Blob* pixelShaderBuffer)
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

bool DefaultBaseShader::InitializeSamplerDesc(ID3D11Device* device)
{
	HRESULT result;
	D3D11_SAMPLER_DESC samplerDesc;

	// Create a texture sampler state description.
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Create the texture sampler state.
	result = device->CreateSamplerState(&samplerDesc, &m_sampleState);
	return !FAILED(result);
}

bool DefaultBaseShader::InitializeConstants(ID3D11Device* device)
{
	HRESULT result;
	D3D11_BUFFER_DESC vertexConstantBufferDesc;
	D3D11_BUFFER_DESC pixelConstantBufferDesc;

	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	vertexConstantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexConstantBufferDesc.ByteWidth = sizeof(VertexConstantBuffer);
	vertexConstantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	vertexConstantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexConstantBufferDesc.MiscFlags = 0;
	vertexConstantBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = device->CreateBuffer(&vertexConstantBufferDesc, NULL, &m_vertexConstantBuffer);
	if (FAILED(result)) {
		return false;
	}

	// Setup the description of the dynamic light constant buffer that is in the pixel shader.
	pixelConstantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	pixelConstantBufferDesc.ByteWidth = sizeof(PixelConstantBuffer);
	pixelConstantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	pixelConstantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	pixelConstantBufferDesc.MiscFlags = 0;
	pixelConstantBufferDesc.StructureByteStride = 0;

	// Create the pixel constant buffer pointer so we can access the pixel shader constant buffer from within this class.
	result = device->CreateBuffer(&pixelConstantBufferDesc, NULL, &m_pixelConstantBuffer);
	return !FAILED(result);
}