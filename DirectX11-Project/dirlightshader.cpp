#include "DirLightShader.h"
#include "scene.h"

DirLightShader::DirLightShader()
{
	wcscpy_s(m_vertexShaderSource, 128, L"../DirectX11-Project/dirlight.vs");
	wcscpy_s(m_pixelShaderSource, 128, L"../DirectX11-Project/dirlight.ps");

	m_vertexShader = 0;
	m_pixelShader = 0;
	m_layout = 0;
	m_sampleState = 0;
	m_vertexConstantBuffer = 0;
	m_pixelConstantBuffer = 0;
}

DirLightShader::DirLightShader(const DirLightShader& other)
{
	m_vertexShader = other.m_vertexShader;
	m_pixelShader = other.m_pixelShader;
	m_vertexConstantBuffer = other.m_vertexConstantBuffer;
	m_pixelConstantBuffer = other.m_pixelConstantBuffer;
	m_sampleState = other.m_sampleState;
	m_layout = other.m_layout;
}

DirLightShader::~DirLightShader()
{
}

bool DirLightShader::Bind(ID3D11DeviceContext* deviceContext, Camera& camera, Model& model, Transform& modelTransform, Light& light, Transform& lightTransform)
{
	bool success;
	VertexConstantBuffer vertexConstantBuffer;
	PixelConstantBuffer pixelConstantBuffer;
	ID3D11ShaderResourceView* texture;

	deviceContext->IASetInputLayout(m_layout);
	deviceContext->VSSetShader(m_vertexShader, NULL, 0);
	deviceContext->PSSetShader(m_pixelShader, NULL, 0);

	// Transpose the matrices to prepare them for the shader.
	vertexConstantBuffer.model      = DirectX::XMMatrixTranspose(modelTransform.GetModelMatrix());
	vertexConstantBuffer.view       = DirectX::XMMatrixTranspose(camera.GetViewMatrix());
	vertexConstantBuffer.projection = DirectX::XMMatrixTranspose(camera.GetProjectionMatrix());

	// Load the directional light data into the buffer.
	DirectX::XMFLOAT3 lightPosition = lightTransform.GetGlobalPosition();
	pixelConstantBuffer.light.position = DirectX::XMFLOAT4(lightPosition.x, lightPosition.y, lightPosition.z, 1.0f);
	pixelConstantBuffer.light.direction = lightTransform.GetForward();
	pixelConstantBuffer.light.color = light.GetColor();

	texture = model.GetTexture()->GetTexture2D();
	
	success = SetShaderParameters(deviceContext, vertexConstantBuffer, pixelConstantBuffer, texture);
	if (!success) {
		return success;
	}

	// Set the sampler state in the pixel shader.
	deviceContext->PSSetSamplers(0, 1, &m_sampleState);

	// Set the blend state in the output merger.
	deviceContext->OMSetBlendState(m_blendState, 0, 0xffffffff);

	return success;
}

bool DirLightShader::IsLit()
{
	return true;
}

bool DirLightShader::SetShaderParameters(ID3D11DeviceContext* deviceContext, VertexConstantBuffer vertexConstantBuffer, PixelConstantBuffer pixelConstantBuffer, ID3D11ShaderResourceView* texture)
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
	vertexDataPtr->model      = vertexConstantBuffer.model;
	vertexDataPtr->view       = vertexConstantBuffer.view;
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

	// Copy the light data into the constant buffer.
	pixelDataPtr->light = pixelConstantBuffer.light;

	// Set the constant buffer in the pixel shader with the updated values.
	deviceContext->PSSetConstantBuffers(bufferNumber, 1, &m_pixelConstantBuffer);

	// Unlock the constant buffer.
	deviceContext->Unmap(m_pixelConstantBuffer, 0);

	// Finally, set shader texture resource in the pixel shader.
	deviceContext->PSSetShaderResources(0, 1, &texture);

	return true;
}

bool DirLightShader::InitializeLayout(ID3D11Device* device, ID3D10Blob* vertexShaderBuffer, ID3D10Blob* pixelShaderBuffer)
{
	HRESULT result;
	unsigned int numElements;

	D3D11_INPUT_ELEMENT_DESC* polygonLayout = CreateLayout(
		true,  // Use position.
		true,  // Use normal.
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

bool DirLightShader::InitializeSamplerDesc(ID3D11Device* device)
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

bool DirLightShader::InitializeBlendDesc(ID3D11Device* device)
{
	HRESULT result;
	D3D11_BLEND_DESC blendDesc;
	D3D11_RENDER_TARGET_BLEND_DESC rtBlendDesc;

	// Create a blend description which will determine how multi-pass rendering will work.
	blendDesc.AlphaToCoverageEnable   = FALSE;
	blendDesc.IndependentBlendEnable  = FALSE;
	rtBlendDesc.BlendEnable           = TRUE;
	rtBlendDesc.SrcBlend              = D3D11_BLEND_ONE;
	rtBlendDesc.DestBlend             = D3D11_BLEND_ONE;
	rtBlendDesc.BlendOp               = D3D11_BLEND_OP_ADD;
	rtBlendDesc.SrcBlendAlpha         = D3D11_BLEND_ONE;
	rtBlendDesc.DestBlendAlpha        = D3D11_BLEND_ONE;
	rtBlendDesc.BlendOpAlpha          = D3D11_BLEND_OP_ADD;
	rtBlendDesc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	blendDesc.RenderTarget[0] = rtBlendDesc;

	// Create the blend state.
	result = device->CreateBlendState(&blendDesc, &m_blendState);
	return !FAILED(result);
}

bool DirLightShader::InitializeConstants(ID3D11Device* device)
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

	// Create the vertex constant buffer pointer so we can access the vertex shader constant buffer from within this class.
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