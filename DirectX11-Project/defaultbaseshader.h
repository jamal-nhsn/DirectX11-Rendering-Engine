#pragma once

/*======
INCLUDES
======*/
#include "shader.h"

class DefaultBaseShader : public Shader
{
private:
	struct MatrixBuffer
	{
		DirectX::XMMATRIX model;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX projection;
	};
	struct AmbientLightBuffer
	{
		DirectX::XMFLOAT4 ambientLight;
	};

public:
	DefaultBaseShader();
	DefaultBaseShader(const DefaultBaseShader&);
	~DefaultBaseShader();

	bool Bind(
		ID3D11DeviceContext* deviceContext,
		DirectX::XMMATRIX modelMatrix,
		DirectX::XMMATRIX viewMatrix,
		DirectX::XMMATRIX projectionMatrix,
		DirectX::XMFLOAT4 ambientLight
	) override;

private:
	bool SetShaderParameters(ID3D11DeviceContext* deviceContext, MatrixBuffer matrixBuffer, AmbientLightBuffer ambientLightBuffer);

protected:
	bool InitializeLayout(ID3D11Device* device, ID3D10Blob* vertexShaderBuffer, ID3D10Blob* pixelShaderBuffer) override;
	bool InitializeConstants(ID3D11Device* device) override;

	void ReleaseBuffers() override;

private:
	ID3D11Buffer* m_matrixBuffer;
	ID3D11Buffer* m_ambientLightBuffer;
};