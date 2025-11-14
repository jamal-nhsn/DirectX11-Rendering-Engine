#include "texture2d.h"

namespace Engine
{
	Texture2D::Texture2D(
		ID3D11Device* device,
		ID3D11DeviceContext* deviceContext,
		unsigned int width,
		unsigned int height,
		DXGI_FORMAT format,
		const void* data,
		unsigned int rowPitch,
		ID3D11SamplerState* samplerState
	)
		: m_width(width)
		, m_height(height)
		, m_samplerState(samplerState)
	{
		// Setup texture description.
		D3D11_TEXTURE2D_DESC textureDesc;
		textureDesc.Width = width;
		textureDesc.Height = height;
		textureDesc.MipLevels = 0;
		textureDesc.ArraySize = 1;
		textureDesc.Format = format;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

		// Create the empty texture.
		HRESULT result = device->CreateTexture2D(&textureDesc, 0, &m_texture);
		if (FAILED(result)) {
			m_texture = 0;
			m_shaderResourceView = 0;
			m_shaderResourceView = 0;
			m_samplerState = 0;
			m_width = 0;
			m_height = 0;
			return;
		}

		deviceContext->UpdateSubresource(m_texture, 0, 0, data, rowPitch, 0);

		// Setup the shader resource view description.
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		srvDesc.Format = textureDesc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = -1;

		// Create the shader resource view for the shader.
		result = device->CreateShaderResourceView(m_texture, &srvDesc, &m_shaderResourceView);
		if (FAILED(result)) {
			m_texture = 0;
			m_shaderResourceView = 0;
			m_shaderResourceView = 0;
			m_samplerState = 0;
			m_width = 0;
			m_height = 0;
			return;
		}

		// Generate mipmaps for the texture
		deviceContext->GenerateMips(m_shaderResourceView);
	}

	Texture2D::Texture2D(Texture2D&& other) noexcept
	{
		m_texture = other.m_texture;
		m_shaderResourceView = other.m_shaderResourceView;
		m_shaderResourceView = other.m_shaderResourceView;
		m_width = other.m_width;
		m_height = other.m_height;
		m_samplerState = other.m_samplerState;

		// Invalidate other.
		other.m_texture = 0;
		other.m_shaderResourceView = 0;
		other.m_shaderResourceView = 0;
		other.m_samplerState = 0;
		other.m_width = 0;
		other.m_height = 0;
	}

	Texture2D& Texture2D::operator=(Texture2D&& other) noexcept
	{
		m_texture = other.m_texture;
		m_shaderResourceView = other.m_shaderResourceView;
		m_shaderResourceView = other.m_shaderResourceView;
		m_width = other.m_width;
		m_height = other.m_height;

		// Invalidate other.
		other.m_texture = 0;
		other.m_shaderResourceView = 0;
		other.m_shaderResourceView = 0;
		other.m_width = 0;
		other.m_height = 0;

		return *this;
	}

	Texture2D::~Texture2D()
	{
		if (m_texture) {
			m_texture->Release();
		}
		if (m_shaderResourceView) {
			m_shaderResourceView->Release();
		}

		// We do not own the sampler state. Do not release it.
	}

	bool Texture2D::Bind(ID3D11DeviceContext* deviceContext)
	{
		// Invalid texture;
		if (!m_shaderResourceView || !m_samplerState) {
			return false;
		}

		deviceContext->PSSetSamplers(0, 1, &m_samplerState);
		deviceContext->PSSetShaderResources(0, 1, &m_shaderResourceView);

		return true;
	}

	unsigned int Texture2D::GetWidth()
	{
		return m_width;
	}

	unsigned int Texture2D::GetHeight()
	{
		return m_height;
	}
}