#include "defaultbaseshader.h"
#include "scene.h"

DefaultBaseShader::DefaultBaseShader()
{
	wcscpy_s(m_vertexShaderSource, 128, L"../DirectX11-Project/defaultbasevertex.hlsl");
	wcscpy_s(m_pixelShaderSource, 128, L"../DirectX11-Project/defaultbasepixel.hlsl");

	m_vertexShader = 0;
	m_pixelShader = 0;
	m_layout = 0;

	m_matrixBuffer = 0;
	m_ambientLightBuffer = 0;
}

DefaultBaseShader::DefaultBaseShader(const DefaultBaseShader& other)
{
	m_vertexShader = other.m_vertexShader;
	m_pixelShader = other.m_pixelShader;
	m_layout = other.m_layout;

	m_matrixBuffer = other.m_matrixBuffer;
	m_ambientLightBuffer = other.m_ambientLightBuffer;
}

DefaultBaseShader::~DefaultBaseShader()
{
}

bool DefaultBaseShader::Bind(
	ID3D11DeviceContext* deviceContext,
	DirectX::XMMATRIX modelMatrix,
	DirectX::XMMATRIX viewMatrix,
	DirectX::XMMATRIX projectionMatrix,
	DirectX::XMFLOAT4 ambientLight
)
{
	bool success;
	AmbientLightBuffer ambientLightBuffer;
	MatrixBuffer matrixBuffer;

	deviceContext->IASetInputLayout(m_layout);
	deviceContext->VSSetShader(m_vertexShader, NULL, 0);
	deviceContext->PSSetShader(m_pixelShader, NULL, 0);

	// Transpose the matrices to prepare them for the shader.
	matrixBuffer.model = DirectX::XMMatrixTranspose(modelMatrix);
	matrixBuffer.view = DirectX::XMMatrixTranspose(viewMatrix);
	matrixBuffer.projection = DirectX::XMMatrixTranspose(projectionMatrix);

	// Pass the ambient light to the pixel shader.
	ambientLightBuffer.ambientLight = ambientLight;

	success = SetShaderParameters(deviceContext, matrixBuffer, ambientLightBuffer);
	if (!success) {
		return success;
	}

	// Finally, set the shader states.
	SetShaderStates(deviceContext);

	return success;
}

bool DefaultBaseShader::SetShaderParameters(ID3D11DeviceContext* deviceContext, MatrixBuffer matrixBuffer, AmbientLightBuffer ambientLightBuffer)
{
	bool success;

	// Set the matrix buffer in the vertex shader with the updated values.
	success = LoadBuffer<MatrixBuffer>(deviceContext, m_matrixBuffer, matrixBuffer);
	if (!success) {
		return success;
	}
	deviceContext->VSSetConstantBuffers(0, 1, &m_matrixBuffer);

	// Set the ambient light buffer in the pixel shader with the updated values.
	success = LoadBuffer<AmbientLightBuffer>(deviceContext, m_ambientLightBuffer, ambientLightBuffer);
	if (!success) {
		return success;
	}
	deviceContext->PSSetConstantBuffers(0, 1, &m_ambientLightBuffer);

	return success;
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