#pragma once

/*======
INCLUDES
======*/
#include <d3d11.h>
#include <stdio.h>

class Texture
{
public:
	Texture();
	Texture(const Texture&);
	~Texture();

	bool Initialize(ID3D11Texture2D* texture, ID3D11ShaderResourceView* textureView, int width, int height);
	void Shutdown();

	ID3D11ShaderResourceView* GetTexture2D();
	ID3D11SamplerState* GetSamplerState();

	void SetSamplerState(ID3D11SamplerState* samplerState);

	int GetWidth();
	int GetHeight();

private:
	ID3D11Texture2D* m_texture;
	ID3D11ShaderResourceView* m_textureView;
	ID3D11SamplerState* m_samplerState;

	int m_width;
	int m_height;
};