#pragma once

/*======
INCLUDES
======*/
#include "shader.h"

class DefaultLightShader : public Shader
{
private:
	struct MatrixBuffer
	{
		DirectX::XMMATRIX model;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX projection;
	};
	struct CameraBuffer
	{
		DirectX::XMFLOAT3 cameraPos;
		float             padding;
	};

	struct LightBuffer
	{
		LightData lightData;
	};
	struct MaterialBuffer
	{
		DirectX::XMFLOAT4 specularTint;
		float             shininess;
		DirectX::XMFLOAT3 padding;
	};

public:
	DefaultLightShader();
	DefaultLightShader(const DefaultLightShader&);
	~DefaultLightShader();

	bool Bind(
		ID3D11DeviceContext* deviceContext,
		DirectX::XMMATRIX modelMatrix,
		DirectX::XMMATRIX viewMatrix,
		DirectX::XMMATRIX projectionMatrix,
		DirectX::XMFLOAT3 cameraPosition,
		DirectX::XMFLOAT4 specularTint,
		float shininess,
		LightData lightData
	) override;

private:
	bool SetShaderParameters(ID3D11DeviceContext* deviceContext, MatrixBuffer matrixBuffer, CameraBuffer cameraBuffer, LightBuffer lightBuffer, MaterialBuffer materialBuffer);

protected:
	bool InitializeLayout(ID3D11Device* device, ID3D10Blob* vertexShaderBuffer, ID3D10Blob* pixelShaderBuffer) override;
	bool InitializeBlendDesc(ID3D11Device* device) override;
	bool InitializeDepthStencilDesc(ID3D11Device* device) override;
	bool InitializeConstants(ID3D11Device* device) override;

	void ReleaseBuffers() override;

private:
	ID3D11Buffer* m_matrixBuffer;
	ID3D11Buffer* m_cameraBuffer;

	ID3D11Buffer* m_lightBuffer;
	ID3D11Buffer* m_materialBuffer;
};