#include "shader.h"

bool Shader::Initialize(ID3D11Device* device, HWND hwnd)
{
	HRESULT result;

	// Compile shaders.

	ID3D10Blob* vertexShaderBuffer = 0;
	ID3D10Blob* pixelShaderBuffer  = 0;
	ID3D10Blob* errorMessage       = 0;

	result = D3DCompileFromFile(
		m_vertexShaderSource,
		NULL,
		NULL,
		"VertexMain",
		"vs_5_0",
		D3D10_SHADER_ENABLE_STRICTNESS,
		0,
		&vertexShaderBuffer,
		&errorMessage
	);

	if (FAILED(result)) {
		// If the shader failed to compile it should have writen something to the error message.
		if (errorMessage) {
			OutputShaderErrorMessage(errorMessage, hwnd, m_vertexShaderSource);
		}
		// If there was  nothing in the error message then it simply could not find the shader file itself.
		else {
			MessageBox(hwnd, m_vertexShaderSource, L"Missing Shader File", MB_OK);
		}
		return false;
	}

	result = D3DCompileFromFile(
		m_pixelShaderSource,
		NULL,
		NULL,
		"PixelMain",
		"ps_5_0",
		D3D10_SHADER_ENABLE_STRICTNESS,
		0,
		&pixelShaderBuffer,
		&errorMessage
	);

	if (FAILED(result)) {
		if (errorMessage) {
			OutputShaderErrorMessage(errorMessage, hwnd, m_pixelShaderSource);
		}
		else {
			MessageBox(hwnd, m_pixelShaderSource, L"Missing Shader File", MB_OK);
		}
		return false;
	}

	// Create the vertex shader from the buffer.
	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_vertexShader);
	if (FAILED(result)) {
		return false;
	}

	// Create the pixel shader from the buffer.
	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pixelShader);
	if (FAILED(result)) {
		return false;
	}

	// Setup layout.

	bool layoutIntialized = InitializeLayout(device, vertexShaderBuffer, pixelShaderBuffer);
	if (!layoutIntialized) {
		return false;
	}

	// Release the vertex shader buffer and pixel shader buffer since they are no longer needed.
	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;

	// Set up constants
	return InitializeConstants(device);
}

void Shader::Shutdown()
{
	// Release the vertex shader.
	if (m_vertexShader) {
		m_vertexShader->Release();
		m_vertexShader = 0;
	}
	// Release the pixel shader.
	if (m_pixelShader) {
		m_pixelShader->Release();
		m_pixelShader = 0;
	}
	// Release the layout.
	if (m_layout) {
		m_layout->Release();
		m_layout = 0;
	}
	// Release the matrix constant buffer.
	if (m_constantBuffer) {
		m_constantBuffer->Release();
		m_constantBuffer = 0;
	}
}

D3D11_INPUT_ELEMENT_DESC* Shader::CreateLayout(bool usePosition, bool useNormal, bool useTexCoord, bool useTangent, bool useColor, unsigned int& numElements)
{
	numElements =
		(usePosition ? 1 : 0) +
		(useNormal   ? 1 : 0) +
		(useTexCoord ? 1 : 0) +
		(useTangent  ? 1 : 0) +
		(useColor    ? 1 : 0);

	// Create the vertex input layout description.
	// This setup needs to match the Vertex stucture.
	D3D11_INPUT_ELEMENT_DESC* polygonLayout = new D3D11_INPUT_ELEMENT_DESC[numElements];

	if (!polygonLayout) {
		return polygonLayout;
	}

	int index = 0;
	size_t offset = 0;

	if (usePosition) {
		polygonLayout[index].SemanticName         = "POSITION";
		polygonLayout[index].SemanticIndex        = 0;
		polygonLayout[index].Format               = DXGI_FORMAT_R32G32B32_FLOAT;
		polygonLayout[index].InputSlot            = 0;
		polygonLayout[index].AlignedByteOffset    = offset;
		polygonLayout[index].InputSlotClass       = D3D11_INPUT_PER_VERTEX_DATA;
		polygonLayout[index].InstanceDataStepRate = 0;

		index++;
	}

	offset += sizeof(DirectX::XMFLOAT3); // Position size.

	if (useNormal) {
		polygonLayout[index].SemanticName         = "NORMAL";
		polygonLayout[index].SemanticIndex        = 0;
		polygonLayout[index].Format               = DXGI_FORMAT_R32G32B32_FLOAT;
		polygonLayout[index].InputSlot            = 0;
		polygonLayout[index].AlignedByteOffset    = offset;
		polygonLayout[index].InputSlotClass       = D3D11_INPUT_PER_VERTEX_DATA;
		polygonLayout[index].InstanceDataStepRate = 0;

		index++;
	}

	offset += sizeof(DirectX::XMFLOAT3); // Normal size.

	if (useTexCoord) {
		polygonLayout[index].SemanticName         = "TEXCOORD";
		polygonLayout[index].SemanticIndex        = 0;
		polygonLayout[index].Format               = DXGI_FORMAT_R32G32_FLOAT;
		polygonLayout[index].InputSlot            = 0;
		polygonLayout[index].AlignedByteOffset    = offset;
		polygonLayout[index].InputSlotClass       = D3D11_INPUT_PER_VERTEX_DATA;
		polygonLayout[index].InstanceDataStepRate = 0;

		index++;
	}

	offset += sizeof(DirectX::XMFLOAT2); // TexCoord size.

	if (useTangent) {
		polygonLayout[index].SemanticName         = "TANGENT";
		polygonLayout[index].SemanticIndex        = 0;
		polygonLayout[index].Format               = DXGI_FORMAT_R32G32B32_FLOAT;
		polygonLayout[index].InputSlot            = 0;
		polygonLayout[index].AlignedByteOffset    = offset;
		polygonLayout[index].InputSlotClass       = D3D11_INPUT_PER_VERTEX_DATA;
		polygonLayout[index].InstanceDataStepRate = 0;

		index++;
	}

	offset += sizeof(DirectX::XMFLOAT3); // Tangent size.

	if (useColor) {
		polygonLayout[index].SemanticName         = "COLOR";
		polygonLayout[index].SemanticIndex        = 0;
		polygonLayout[index].Format               = DXGI_FORMAT_R32G32B32A32_FLOAT;
		polygonLayout[index].InputSlot            = 0;
		polygonLayout[index].AlignedByteOffset    = offset;
		polygonLayout[index].InputSlotClass       = D3D11_INPUT_PER_VERTEX_DATA;
		polygonLayout[index].InstanceDataStepRate = 0;
	}

	return polygonLayout;
}

void Shader::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderSource)
{
	// Get a pointer to the error message text buffer.
	char* compileErrors = (char*)(errorMessage->GetBufferPointer());

	// Get the length of the message.
	unsigned long long bufferSize = errorMessage->GetBufferSize();

	// Open a file to write the error message to.
	std::ofstream fout;
	fout.open("shader-error.txt", std::ios::binary);

	// Write out the error message.
	fout.write(compileErrors, static_cast<std::streamsize>(bufferSize));

	// Close the file.
	fout.close();

	// Release the error message.
	errorMessage->Release();
	errorMessage = 0;

	// Pop a message up on the screen to notify the user to check the text file for compile errors.
	MessageBox(hwnd, L"Error compiling shader.  Check shader-error.txt for message.", shaderSource, MB_OK);
}