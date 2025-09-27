#pragma once

/*======
INCLUDES
======*/
#include "shader.h"
#include "light.h"

class DirLightShader : public Shader
{
private:
	struct VertexConstantBuffer
	{
		DirectX::XMMATRIX model;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX projection;
	};
	struct PixelConstantBuffer
	{
		LightStruct light;
	};

public:
	DirLightShader();
	DirLightShader(const DirLightShader&);
	~DirLightShader();

	bool Bind(ID3D11DeviceContext* deviceContext, Scene* scene, int entity) override;
	bool SetShaderParameters(ID3D11DeviceContext* deviceContext, VertexConstantBuffer vertexConstantBuffer, PixelConstantBuffer pixelConstantBuffer, ID3D11ShaderResourceView* texture);

protected:
	bool InitializeLayout(ID3D11Device* device, ID3D10Blob* vertexShaderBuffer, ID3D10Blob* pixelShaderBuffer) override;
	bool InitializeSamplerDesc(ID3D11Device* device) override;
	bool InitializeConstants(ID3D11Device* device) override;
};