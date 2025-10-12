#include "colorshader.h"
#include "scene.h"

ColorShader::ColorShader()
{
	wcscpy_s(m_vertexShaderSource, 128, L"../DirectX11-Project/colorvertex.hlsl");
	wcscpy_s(m_pixelShaderSource,  128, L"../DirectX11-Project/colorpixel.hlsl");

	m_vertexShader   = 0;
	m_pixelShader    = 0;
	m_layout         = 0;

	m_matrixBuffer = 0;
}

ColorShader::ColorShader(const ColorShader& other)
{
	m_vertexShader = other.m_vertexShader;
	m_pixelShader = other.m_pixelShader;
	m_layout = other.m_layout;

	m_matrixBuffer = other.m_matrixBuffer;
}

ColorShader::~ColorShader()
{
}

bool ColorShader::Bind(
	ID3D11DeviceContext* deviceContext,
	DirectX::XMMATRIX modelMatrix,
	DirectX::XMMATRIX viewMatrix,
	DirectX::XMMATRIX projectionMatrix,
	DirectX::XMFLOAT4 ambientLight
)
{
	bool success;
	MatrixBuffer matrixBuffer;

	deviceContext->IASetInputLayout(m_layout);
	deviceContext->VSSetShader(m_vertexShader, NULL, 0);
	deviceContext->PSSetShader(m_pixelShader, NULL, 0);

	// Transpose the matrices to prepare them for the shader.
	matrixBuffer.model = DirectX::XMMatrixTranspose(modelMatrix);
	matrixBuffer.view = DirectX::XMMatrixTranspose(viewMatrix);
	matrixBuffer.projection = DirectX::XMMatrixTranspose(projectionMatrix);

	success = SetShaderParameters(deviceContext, matrixBuffer);
	if (!success) {
		return success;
	}

	// Finally, set the shader states.
	SetShaderStates(deviceContext);

	return success;
}

bool ColorShader::SetShaderParameters(ID3D11DeviceContext* deviceContext, MatrixBuffer matrixBuffer)
{
	bool success;

	// Set the matrix buffer in the vertex shader with the updated values.
	success = LoadBuffer<MatrixBuffer>(deviceContext, m_matrixBuffer, matrixBuffer);
	if (!success) {
		return false;
	}
	deviceContext->VSSetConstantBuffers(0, 1, &m_matrixBuffer);

	return true;
}

bool ColorShader::InitializeLayout(ID3D11Device* device, ID3D10Blob* vertexShaderBuffer, ID3D10Blob* pixelShaderBuffer)
{
	HRESULT result;
	unsigned int numElements;

	D3D11_INPUT_ELEMENT_DESC* polygonLayout = CreateLayout(
		true,  // Use position.
		false, // Use normal.
		false, // Use texcoord.
		false, // Use tangent.
		true,  // Use color.
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

bool ColorShader::InitializeConstants(ID3D11Device* device)
{
	HRESULT result;
	D3D11_BUFFER_DESC constantBufferDesc;

	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	constantBufferDesc.Usage               = D3D11_USAGE_DYNAMIC;
	constantBufferDesc.ByteWidth           = sizeof(MatrixBuffer);
	constantBufferDesc.BindFlags           = D3D11_BIND_CONSTANT_BUFFER;
	constantBufferDesc.CPUAccessFlags      = D3D11_CPU_ACCESS_WRITE;
	constantBufferDesc.MiscFlags           = 0;
	constantBufferDesc.StructureByteStride = 0;

	// Create the matrix buffer pointer so we can access the matrix buffer from the vertex shader.
	result = device->CreateBuffer(&constantBufferDesc, NULL, &m_matrixBuffer);
	return !FAILED(result);
}

void ColorShader::ReleaseBuffers()
{
	// Release matrix buffer.
	if (m_matrixBuffer) {
		m_matrixBuffer->Release();
		m_matrixBuffer = 0;
	}

}