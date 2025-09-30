#pragma once

/*======
INCLUDES
======*/
#include "shader.h"

class TextureShader : public Shader
{
private:
	struct MatrixBuffer
	{
		DirectX::XMMATRIX model;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX projection;
	};

public:
	TextureShader();
	TextureShader(const TextureShader&);
	~TextureShader();

	bool Bind(ID3D11DeviceContext* deviceContext, Camera& camera, Model& model, Transform& modelTransform, DirectX::XMFLOAT4 ambientLight) override;

private:
	bool SetShaderParameters(ID3D11DeviceContext* deviceContext, MatrixBuffer matrixBuffer, ID3D11ShaderResourceView* texture);

protected:
	bool InitializeLayout(ID3D11Device* device, ID3D10Blob* vertexShaderBuffer, ID3D10Blob* pixelShaderBuffer) override;
	bool InitializeSamplerDesc(ID3D11Device* device) override;
	bool InitializeConstants(ID3D11Device* device) override;

	void ReleaseBuffers() override;

private:
	ID3D11Buffer* m_matrixBuffer;
};