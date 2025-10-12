#pragma once

/*======
INCLUDES
======*/
#include "shader.h"

class DefaultSpriteShader : public Shader
{
private:
	struct MatrixBuffer
	{
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX projection;
	};

public:
	DefaultSpriteShader();
	DefaultSpriteShader(const DefaultSpriteShader&);
	~DefaultSpriteShader();
	
	bool Bind(
		ID3D11DeviceContext* deviceContext,
		DirectX::XMMATRIX viewMatrix,
		DirectX::XMMATRIX projectionMatrix
	) override;

private:
	bool SetShaderParameters(ID3D11DeviceContext* deviceContext, MatrixBuffer matrixBuffer);

protected:
	bool InitializeLayout(ID3D11Device* device, ID3D10Blob* vertexShaderBuffer, ID3D10Blob* pixelShaderBuffer) override;
	bool InitializeConstants(ID3D11Device* device) override;

	void ReleaseBuffers() override;

private:
	ID3D11Buffer* m_matrixBuffer;
};