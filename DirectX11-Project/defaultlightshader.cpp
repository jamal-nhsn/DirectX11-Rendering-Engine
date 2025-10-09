#include "defaultlightshader.h"
#include "scene.h"

DefaultLightShader::DefaultLightShader()
{
	wcscpy_s(m_vertexShaderSource, 128, L"../DirectX11-Project/defaultlightvertex.hlsl");
	wcscpy_s(m_pixelShaderSource, 128, L"../DirectX11-Project/defaultlightpixel.hlsl");

	m_vertexShader = 0;
	m_pixelShader = 0;
	m_layout = 0;
	m_sampleState = 0;

	m_matrixBuffer = 0;
	m_cameraBuffer = 0;

	m_lightBuffer = 0;
	m_materialBuffer = 0;
}

DefaultLightShader::DefaultLightShader(const DefaultLightShader& other)
{
	m_vertexShader = other.m_vertexShader;
	m_pixelShader = other.m_pixelShader;
	m_sampleState = other.m_sampleState;
	m_layout = other.m_layout;

	m_matrixBuffer = other.m_materialBuffer;
	m_cameraBuffer = other.m_cameraBuffer;

	m_lightBuffer = other.m_lightBuffer;
	m_materialBuffer = other.m_materialBuffer;
}

DefaultLightShader::~DefaultLightShader()
{
}

bool DefaultLightShader::Bind(ID3D11DeviceContext* deviceContext, Camera3D& camera3D, Transform& cameraTransform, Model& model, Transform& modelTransform, Light& light, Transform& lightTransform)
{
	bool success;
	MatrixBuffer matrixBuffer;
	CameraBuffer cameraBuffer;
	LightBuffer lightBuffer;
	MaterialBuffer materialBuffer;
	ID3D11ShaderResourceView* texture;

	deviceContext->IASetInputLayout(m_layout);
	deviceContext->VSSetShader(m_vertexShader, NULL, 0);
	deviceContext->PSSetShader(m_pixelShader, NULL, 0);

	// Transpose the matrices to prepare them for the shader.
	matrixBuffer.model      = DirectX::XMMatrixTranspose(modelTransform.GetModelMatrix());
	matrixBuffer.view       = DirectX::XMMatrixTranspose(camera3D.GetViewMatrix());
	matrixBuffer.projection = DirectX::XMMatrixTranspose(camera3D.GetProjectionMatrix());

	// Get the cameras position.
	cameraBuffer.cameraPos = cameraTransform.GetGlobalPosition();

	// Load the light data into the buffer.
	DirectX::XMFLOAT3 lightPosition = lightTransform.GetGlobalPosition();
	DirectX::XMFLOAT3 lightDirection = lightTransform.GetForward();
	light.FillLightData(&lightBuffer.lightData, lightPosition, lightDirection);

	// Load the material data into the buffer.
	materialBuffer.specularTint = model.GetSpecularTint();
	materialBuffer.shininess = model.GetShininess();

	texture = model.GetTexture()->GetTexture2D();
	
	success = SetShaderParameters(deviceContext, matrixBuffer, cameraBuffer, lightBuffer, materialBuffer, texture);
	if (!success) {
		return success;
	}

	// Finally, set the shader states.
	SetShaderStates(deviceContext);

	return success;
}

bool DefaultLightShader::SetShaderParameters(ID3D11DeviceContext* deviceContext, MatrixBuffer matrixBuffer, CameraBuffer cameraBuffer, LightBuffer lightBuffer, MaterialBuffer materialBuffer, ID3D11ShaderResourceView* texture)
{	
	bool success;

	// Set the buffers in the vertex shader with the updated values.
	ID3D11Buffer* vertexConstantBuffers[] = { m_matrixBuffer, m_cameraBuffer };
	success = LoadBuffer<MatrixBuffer>(deviceContext, m_matrixBuffer, matrixBuffer);
	success = success && LoadBuffer<CameraBuffer>(deviceContext, m_cameraBuffer, cameraBuffer);
	if (!success) {
		return false;
	}
	deviceContext->VSSetConstantBuffers(0, 2, vertexConstantBuffers);

	// Set the buffers in the pixel shader with the updated values.
	ID3D11Buffer* pixelConstantBuffers[] = { m_lightBuffer, m_materialBuffer };
	success = LoadBuffer<LightBuffer>(deviceContext, m_lightBuffer, lightBuffer);
	success = success && LoadBuffer<MaterialBuffer>(deviceContext, m_materialBuffer, materialBuffer);
	if (!success) {
		return false;
	}
	deviceContext->PSSetConstantBuffers(0, 2, pixelConstantBuffers);

	// Finally, set shader texture resource in the pixel shader.
	deviceContext->PSSetShaderResources(0, 1, &texture);

	return true;
}

bool DefaultLightShader::InitializeLayout(ID3D11Device* device, ID3D10Blob* vertexShaderBuffer, ID3D10Blob* pixelShaderBuffer)
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

bool DefaultLightShader::InitializeSamplerDesc(ID3D11Device* device)
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

bool DefaultLightShader::InitializeBlendDesc(ID3D11Device* device)
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

bool DefaultLightShader::InitializeDepthStencilDesc(ID3D11Device* device)
{
	HRESULT result;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;

	// Create a depth stencil description, which will determine how depth writes work.
	depthStencilDesc.DepthEnable    = TRUE;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	depthStencilDesc.DepthFunc      = D3D11_COMPARISON_EQUAL;
	depthStencilDesc.StencilEnable  = FALSE;

	result = device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState);
	return !FAILED(result);
}

bool DefaultLightShader::InitializeConstants(ID3D11Device* device)
{
	HRESULT result;
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_BUFFER_DESC cameraBufferDesc;
	D3D11_BUFFER_DESC lightBufferDesc;
	D3D11_BUFFER_DESC materialBufferDesc;

	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBuffer);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// Create the matrix buffer pointer so we can access the matrix buffer in the vertex shader.
	result = device->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);
	if (FAILED(result)) {
		return false;
	}

	// Setup the description of the dynamic camera constant buffer that is in the vertex shader.
	cameraBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	cameraBufferDesc.ByteWidth = sizeof(CameraBuffer);
	cameraBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cameraBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cameraBufferDesc.MiscFlags = 0;
	cameraBufferDesc.StructureByteStride = 0;

	// Create the camera buffer pointer so we can acess the camera buffer in the vertex shader.
	result = device->CreateBuffer(&cameraBufferDesc, NULL, &m_cameraBuffer);
	if (FAILED(result)) {
		return false;
	}

	// Setup the description of the dynamic light constant buffer that is in the pixel shader.
	lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.ByteWidth = sizeof(LightBuffer);
	lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags = 0;
	lightBufferDesc.StructureByteStride = 0;

	// Create the light buffer pointer so we can access light buffer in the pixel shader.
	result = device->CreateBuffer(&lightBufferDesc, NULL, &m_lightBuffer);
	if (FAILED(result)) {
		return false;
	}

	// Setup the description of the dynamic material constant buffer that is in the pixel shader.
	materialBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	materialBufferDesc.ByteWidth = sizeof(MaterialBuffer);
	materialBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	materialBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	materialBufferDesc.MiscFlags = 0;
	materialBufferDesc.StructureByteStride = 0;

	// Create the material buffer pointer so we can acess the material buffer in the pixel shader.
	result = device->CreateBuffer(&materialBufferDesc, NULL, &m_materialBuffer);
	return !FAILED(result);
}

void DefaultLightShader::ReleaseBuffers()
{
	// Release matrix buffer.
	if (m_matrixBuffer) {
		m_matrixBuffer->Release();
		m_matrixBuffer = 0;
	}

	// Release light buffer.
	if (m_lightBuffer) {
		m_lightBuffer->Release();
		m_lightBuffer = 0;
	}
}