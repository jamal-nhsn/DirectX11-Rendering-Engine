#pragma once

/*======
INCLUDES
======*/
#include <directxmath.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <fstream>

class Scene;

class Shader
{
public:
	bool Initialize(ID3D11Device* device, HWND hwnd);
	virtual void Bind(ID3D11DeviceContext* deviceContext, Scene* scene, int entity) = 0;
	void Shutdown();

protected:
	D3D11_INPUT_ELEMENT_DESC* CreateLayout(bool usePosition, bool useNormal, bool useTexCoord, bool useTangent, bool useColor, unsigned int& numElements);
	void OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderSource);

	virtual bool InitializeLayout(ID3D11Device* device, ID3D10Blob* vertexShaderBuffer, ID3D10Blob* pixelShaderBuffer) = 0;
	virtual bool InitializeSamplerDesc(ID3D11Device* device) = 0;
	virtual bool InitializeConstants(ID3D11Device* device) = 0;

protected:
	wchar_t m_vertexShaderSource[128];
	wchar_t m_pixelShaderSource[128];

	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader*  m_pixelShader;
	ID3D11InputLayout*  m_layout;
	ID3D11SamplerState* m_sampleState;
	ID3D11Buffer*       m_constantBuffer;
};