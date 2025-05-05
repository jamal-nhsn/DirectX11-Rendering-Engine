#include "colorshader.h"

ColorShader::ColorShader()
{
	wcscpy_s(m_vertexShaderSource, 128, L"../DirectX11-Project/color.vs");
	wcscpy_s(m_pixelShaderSource,  128, L"../DirectX11-Project/color.ps");

	m_vertexShader   = 0;
	m_pixelShader    = 0;
	m_layout         = 0;
	m_constantBuffer = 0;
}

ColorShader::ColorShader(const ColorShader& other)
{
}

ColorShader::~ColorShader()
{
}

bool ColorShader::SetShaderParameters(ID3D11DeviceContext* deviceContext, DirectX::XMMATRIX modelMatrix, DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX projectionMatrix)
{
	HRESULT result;

	// Transpose the matrices to prepare them for the shader.
	modelMatrix      = DirectX::XMMatrixTranspose(modelMatrix);
	viewMatrix       = DirectX::XMMatrixTranspose(viewMatrix);
	projectionMatrix = DirectX::XMMatrixTranspose(projectionMatrix);

	// Lock the constant buffer so it can be written to.
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	result = deviceContext->Map(m_constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result)) {
		return false;
	}

	// Get a pointer to the data in the constant buffer.
	ConstantBuffer* dataPtr;
	dataPtr = (ConstantBuffer*)mappedResource.pData;

	// Copy the matrices into the constant buffer.
	dataPtr->model      = modelMatrix;
	dataPtr->view       = viewMatrix;
	dataPtr->projection = projectionMatrix;

	// Unlock the constant buffer.
	deviceContext->Unmap(m_constantBuffer, 0);

	// Set the position of the constant buffer in the vertex shader.
	unsigned int bufferNumber = 0;

	// Finally, set the constant buffer in the vertex shader with the updated values.
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_constantBuffer);

	return true;
}

bool ColorShader::InitializeLayout(ID3D11Device* device, ID3D10Blob* vertexShaderBuffer, ID3D10Blob* pixelShaderBuffer)
{
	HRESULT result;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];

	// Create the vertex input layout description.
	// This setup needs to match the VertexType stucture in the ModelClass and in the shader.
	polygonLayout[0].SemanticName         = "POSITION";
	polygonLayout[0].SemanticIndex        = 0;
	polygonLayout[0].Format               = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot            = 0;
	polygonLayout[0].AlignedByteOffset    = 0;
	polygonLayout[0].InputSlotClass       = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName         = "COLOR";
	polygonLayout[1].SemanticIndex        = 0;
	polygonLayout[1].Format               = DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygonLayout[1].InputSlot            = 0;
	polygonLayout[1].AlignedByteOffset    = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass       = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	// Get a count of the elements in the layout.
	unsigned int numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// Create the vertex input layout.
	result = device->CreateInputLayout(
		polygonLayout,
		numElements,
		vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(),
		&m_layout
	);

	return !FAILED(result);
}

bool ColorShader::InitializeConstants(ID3D11Device* device)
{
	HRESULT result;
	D3D11_BUFFER_DESC constantBufferDesc;

	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	constantBufferDesc.Usage               = D3D11_USAGE_DYNAMIC;
	constantBufferDesc.ByteWidth           = sizeof(ConstantBuffer);
	constantBufferDesc.BindFlags           = D3D11_BIND_CONSTANT_BUFFER;
	constantBufferDesc.CPUAccessFlags      = D3D11_CPU_ACCESS_WRITE;
	constantBufferDesc.MiscFlags           = 0;
	constantBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = device->CreateBuffer(&constantBufferDesc, NULL, &m_constantBuffer);
	return !FAILED(result);
}