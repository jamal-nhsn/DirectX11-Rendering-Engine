#pragma once

/*======
INCLUDES
======*/
#include "shader.h"

class ColorShader : public Shader
{
private:
	struct VertexConstantBuffer
	{
		DirectX::XMMATRIX model;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX projection;
	};

public:
	ColorShader();
	ColorShader(const ColorShader&);
	~ColorShader();

	bool Bind(ID3D11DeviceContext* deviceContext, Camera& camera, Model& model, Transform& modelTransform, DirectX::XMFLOAT4 ambientLight) override;

private:
	bool SetShaderParameters(ID3D11DeviceContext* deviceContext, VertexConstantBuffer vertexConstantBuffer);

protected:
	bool InitializeLayout(ID3D11Device* device, ID3D10Blob* vertexShaderBuffer, ID3D10Blob* pixelShaderBuffer) override;
	bool InitializeConstants(ID3D11Device* device) override;
};