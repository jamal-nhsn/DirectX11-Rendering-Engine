#include "defaultbaseshader.h"
#include "scene.h"

DefaultBaseShader::DefaultBaseShader()
{
	wcscpy_s(m_vertexShaderSource, 128, L"../DirectX11-Project/defaultbasevertex.hlsl");
	wcscpy_s(m_pixelShaderSource, 128, L"../DirectX11-Project/defaultbasepixel.hlsl");

	m_vertexShader = 0;
	m_pixelShader = 0;
	m_layout = 0;
	m_sampleState = 0;

	m_matrixBuffer = 0;
	m_ambientLightBuffer = 0;
}

DefaultBaseShader::DefaultBaseShader(const DefaultBaseShader& other)
{
	m_vertexShader = other.m_vertexShader;
	m_pixelShader = other.m_pixelShader;
	m_sampleState = other.m_sampleState;
	m_layout = other.m_layout;

	m_matrixBuffer = other.m_matrixBuffer;
	m_ambientLightBuffer = other.m_ambientLightBuffer;
}

DefaultBaseShader::~DefaultBaseShader()
{
}

bool DefaultBaseShader::Bind(ID3D11DeviceContext* deviceContext, Camera& camera, Model& model, Transform& modelTransform, DirectX::XMFLOAT4 ambientLight)
{
	bool success;
	MatrixBuffer matrixBuffer;
	AmbientLightBuffer ambientLightBuffer;
	ID3D11ShaderResourceView* texture;

	deviceContext->IASetInputLayout(m_layout);
	deviceContext->VSSetShader(m_vertexShader, NULL, 0);
	deviceContext->PSSetShader(m_pixelShader, NULL, 0);

	// Transpose the matrices to prepare them for the shader.
	matrixBuffer.model = DirectX::XMMatrixTranspose(modelTransform.GetModelMatrix());
	matrixBuffer.view = DirectX::XMMatrixTranspose(camera.GetViewMatrix());
	matrixBuffer.projection = DirectX::XMMatrixTranspose(camera.GetProjectionMatrix());

	// Pass the ambient light to the pixel shader.
	ambientLightBuffer.ambientLight = ambientLight;

	texture = model.GetTexture()->GetTexture2D();

	success = SetShaderParameters(deviceContext, matrixBuffer, ambientLightBuffer, texture);
	if (!success) {
		return success;
	}

	// Finally, set the shader states.
	SetShaderStates(deviceContext);

	return success;
}

bool DefaultBaseShader::SetShaderParameters(ID3D11DeviceContext* deviceContext, MatrixBuffer matrixBuffer, AmbientLightBuffer ambientLightBuffer, ID3D11ShaderResourceView* texture)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;

	// Lock the matrix buffer so it can be written to.
	result = deviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result)) {
		return false;
	}
	// Copy the matrix data into the buffer
	MatrixBuffer* matrixDataPtr;
	matrixDataPtr = (MatrixBuffer*)mappedResource.pData;
	*matrixDataPtr = matrixBuffer;
	// Unlock the matrix buffer.
	deviceContext->Unmap(m_matrixBuffer, 0);

	// Lock the ambient light buffer so it can be written to.
	result = deviceContext->Map(m_ambientLightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result)) {
		return false;
	}
	// Copy the ambient light data into the buffer.
	AmbientLightBuffer* ambientLightDataPtr;
	ambientLightDataPtr = (AmbientLightBuffer*)mappedResource.pData;
	*ambientLightDataPtr = ambientLightBuffer;
	// Unlock the ambient light buffer.
	deviceContext->Unmap(m_ambientLightBuffer, 0);

	// Set the ambient light buffer in the pixel shader with the updated values.
	deviceContext->PSSetConstantBuffers(0, 1, &m_ambientLightBuffer);

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
	D3D11_BUFFER_DESC matrixConstantBufferDesc;
	D3D11_BUFFER_DESC ambientLightBufferDesc;

	// Setup the description of the dynamic matrix buffer that is in the vertex shader.
	matrixConstantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixConstantBufferDesc.ByteWidth = sizeof(MatrixBuffer);
	matrixConstantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixConstantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixConstantBufferDesc.MiscFlags = 0;
	matrixConstantBufferDesc.StructureByteStride = 0;

	// Create the matrix buffer pointer so we can access the matrix buffer in the vertex shader.
	result = device->CreateBuffer(&matrixConstantBufferDesc, NULL, &m_matrixBuffer);
	if (FAILED(result)) {
		return false;
	}

	// Setup the description of the dynamic light buffer that is in the pixel shader.
	ambientLightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	ambientLightBufferDesc.ByteWidth = sizeof(AmbientLightBuffer);
	ambientLightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	ambientLightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	ambientLightBufferDesc.MiscFlags = 0;
	ambientLightBufferDesc.StructureByteStride = 0;

	// Create the ambient light buffer pointer so we can access the ambient light buffer in the pixel shader.
	result = device->CreateBuffer(&ambientLightBufferDesc, NULL, &m_ambientLightBuffer);
	return !FAILED(result);
}

void DefaultBaseShader::ReleaseBuffers() 
{
	// Release matrix buffer.
	if (m_matrixBuffer) {
		m_matrixBuffer->Release();
		m_matrixBuffer = 0;
	}

	// Release ambient light buffer.
	if (m_ambientLightBuffer) {
		m_ambientLightBuffer->Release();
		m_ambientLightBuffer = 0;
	}
}