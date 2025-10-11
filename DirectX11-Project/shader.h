#pragma once

/*======
INCLUDES
======*/
#include <directxmath.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <fstream>

#include "camera3d.h"
#include "transform.h"
#include "light.h"

class Model;

class Shader
{
public:
	bool Initialize(ID3D11Device* device, HWND hwnd);
	virtual bool Bind(ID3D11DeviceContext* deviceContext, Camera3D& camera3D, Transform& cameraTransform, Model& model, Transform& modelTransform, Light& light, Transform& lightTransform);
	virtual bool Bind(ID3D11DeviceContext* deviceContext, Camera3D& camera3D, Model& model, Transform& modelTransform, DirectX::XMFLOAT4 ambientLight);
	void Shutdown();

protected:
	D3D11_INPUT_ELEMENT_DESC* CreateLayout(bool usePosition, bool useNormal, bool useTexCoord, bool useTangent, bool useColor, unsigned int& numElements);
	void SetShaderStates(ID3D11DeviceContext* device);
	void OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderSource);

	virtual bool InitializeLayout(ID3D11Device* device, ID3D10Blob* vertexShaderBuffer, ID3D10Blob* pixelShaderBuffer) = 0;
	virtual bool InitializeBlendDesc(ID3D11Device* device);
	virtual bool InitializeDepthStencilDesc(ID3D11Device* device);
	virtual bool InitializeConstants(ID3D11Device* device) = 0;

	template<typename BufferType>
	bool LoadBuffer(ID3D11DeviceContext* deviceContext, ID3D11Buffer* buffer, BufferType data);

	virtual void ReleaseBuffers() = 0;

protected:
	wchar_t m_vertexShaderSource[128];
	wchar_t m_pixelShaderSource[128];

	ID3D11VertexShader*      m_vertexShader;
	ID3D11PixelShader*       m_pixelShader;
	ID3D11InputLayout*       m_layout;
	ID3D11BlendState*        m_blendState;
	ID3D11DepthStencilState* m_depthStencilState;
};

template<typename BufferType>
bool Shader::LoadBuffer(ID3D11DeviceContext* deviceContext, ID3D11Buffer* buffer, BufferType data)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;

	// Lock the buffer so it can be written to.
	result = deviceContext->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result)) {
		return false;
	}
	// Copy the data into the buffer
	BufferType* bufferDataPtr;
	bufferDataPtr = (BufferType*)mappedResource.pData;
	*bufferDataPtr = data;
	// Unlock the buffer.
	deviceContext->Unmap(buffer, 0);

	return true;
}