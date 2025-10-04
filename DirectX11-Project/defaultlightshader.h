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

	bool Bind(ID3D11DeviceContext* deviceContext, Camera& camera, Transform& cameraTransform, Model& model, Transform& modelTransform, Light& light, Transform& lightTransform) override;

private:
	bool SetShaderParameters(ID3D11DeviceContext* deviceContext, MatrixBuffer matrixBuffer, CameraBuffer cameraBuffer, LightBuffer lightBuffer, MaterialBuffer materialBuffer, ID3D11ShaderResourceView* texture);

protected:
	bool InitializeLayout(ID3D11Device* device, ID3D10Blob* vertexShaderBuffer, ID3D10Blob* pixelShaderBuffer) override;
	bool InitializeSamplerDesc(ID3D11Device* device) override;
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