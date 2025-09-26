#pragma once

/*======
INCLUDES
======*/
#include "shader.h"

class ColorShader : public Shader
{
private:
	struct ConstantBuffer
	{
		DirectX::XMMATRIX model;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX projection;
	};

public:
	ColorShader();
	ColorShader(const ColorShader&);
	~ColorShader();

	void Bind(ID3D11DeviceContext* deviceContext, Scene* scene, int entity) override;
	bool SetShaderParameters(ID3D11DeviceContext* deviceContext, DirectX::XMMATRIX modelMatrix, DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX projectionMatrix);

protected:
	bool InitializeLayout(ID3D11Device* device, ID3D10Blob* vertexShaderBuffer, ID3D10Blob* pixelShaderBuffer) override;
	virtual bool InitializeSamplerDesc(ID3D11Device* device) override;
	bool InitializeConstants(ID3D11Device* device) override;
};