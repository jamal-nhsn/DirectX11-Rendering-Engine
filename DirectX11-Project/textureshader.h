#pragma once

/*======
INCLUDES
======*/
#include "shader.h"

class TextureShader : public Shader
{
private:
	struct ConstantBuffer
	{
		DirectX::XMMATRIX model;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX projection;
	};

public:
	TextureShader();
	TextureShader(const TextureShader&);
	~TextureShader();

	void Bind(ID3D11DeviceContext* deviceContext, Camera& camera, Transform& transform, Material* material) override;
	bool SetShaderParameters(ID3D11DeviceContext* deviceContext, DirectX::XMMATRIX modelMatrix, DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture);

protected:
	bool InitializeLayout(ID3D11Device* device, ID3D10Blob* vertexShaderBuffer, ID3D10Blob* pixelShaderBuffer) override;
	bool InitializeSamplerDesc(ID3D11Device* device) override;
	bool InitializeConstants(ID3D11Device* device) override;
};