#include "texture.h"

Texture::Texture()
{
	m_texture     = 0;
	m_textureView = 0;
	m_samplerState = 0;

	m_width  = 0;
	m_height = 0;
}

Texture::Texture(const Texture& other)
{
	m_texture     = other.m_texture;
	m_textureView = other.m_textureView;
	m_samplerState = other.m_samplerState;

	m_width  = other.m_width;
	m_height = other.m_height;
}

Texture::~Texture()
{
}

bool Texture::Initialize(ID3D11Texture2D* texture, ID3D11ShaderResourceView* textureView, int width, int height)
{
	if (!texture || !textureView) {
		return false;
	}
	m_texture = texture;
	m_textureView = textureView;
	m_width = width;
	m_height = height;

	return true;
}

void Texture::Shutdown()
{
	// Release the texture view resource
	if (m_textureView) {
		m_textureView->Release();
		m_textureView = 0;
	}

	// Release the texture
	if (m_texture) {
		m_texture->Release();
		m_texture = 0;
	}

	// Release the sampler state
	if (m_samplerState) {
		m_samplerState->Release();
		m_samplerState;
	}
}

ID3D11ShaderResourceView* Texture::GetTexture2D()
{
	return m_textureView;
}

ID3D11SamplerState* Texture::GetSamplerState()
{
	return m_samplerState;
}

void Texture::SetSamplerState(ID3D11SamplerState* samplerState)
{
	m_samplerState = samplerState;
}

int Texture::GetWidth()
{
	return m_width;
}

int Texture::GetHeight()
{
	return m_height;
}