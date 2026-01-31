#include "pch.h"
#include "texture2d.h"

namespace Engine
{
	Texture2D::Texture2D(const void* data, unsigned int width, unsigned int height, ID3D11SamplerState* samplerState)
		: m_texture(0)
		, m_shaderResourceView(0)
		, m_samplerState(samplerState)
		, m_data(width * height * 4)
		, m_width(width)
		, m_height(height)
	{
		std::memcpy(m_data.data(), data, m_data.size());
	}

	Texture2D::Texture2D(Texture2D&& other) noexcept
	{
		m_texture = other.m_texture;
		m_shaderResourceView = other.m_shaderResourceView;
		m_samplerState = other.m_samplerState;
		m_data.swap(other.m_data);
		m_width = other.m_width;
		m_height = other.m_height;

		// Invalidate other.
		other.m_texture = 0;
		other.m_shaderResourceView = 0;
	}

	Texture2D& Texture2D::operator=(Texture2D&& other) noexcept
	{
		m_texture = other.m_texture;
		m_shaderResourceView = other.m_shaderResourceView;
		m_samplerState = other.m_samplerState;
		m_data.swap(other.m_data);
		m_width = other.m_width;
		m_height = other.m_height;

		// Invalidate other.
		other.m_texture = 0;
		other.m_shaderResourceView = 0;

		return *this;
	}

	Texture2D::~Texture2D()
	{
		Release();
	}

	void Texture2D::Upload(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
	{
		// Already uploaded.
		if (m_texture && m_shaderResourceView) {
			return;
		}

		// Setup texture description.
		D3D11_TEXTURE2D_DESC textureDesc;
		textureDesc.Width = m_width;
		textureDesc.Height = m_height;
		textureDesc.MipLevels = 0;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

		// Create the empty texture.
		HRESULT result = device->CreateTexture2D(&textureDesc, 0, &m_texture);
		assert(("Error: Could not upload Texture2D!", !FAILED(result)));

		deviceContext->UpdateSubresource(m_texture, 0, 0, m_data.data(), m_width * 4, 0);

		// Setup the shader resource view description.
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = -1;

		// Create the shader resource view for the shader.
		result = device->CreateShaderResourceView(m_texture, &srvDesc, &m_shaderResourceView);
		assert(("Error: Could not upload Texture2D!", !FAILED(result)));

		// Generate mipmaps for the texture
		deviceContext->GenerateMips(m_shaderResourceView);
	}

	void Texture2D::Release()
	{
		if (m_texture) {
			m_texture->Release();
		}
		if (m_shaderResourceView) {
			m_shaderResourceView->Release();
		}

		// We do not own the sampler state. Do not release it.
	}

	void Texture2D::Bind(ID3D11DeviceContext* deviceContext)
	{
		// Ensure the texture was uploaded.
		assert(("Error: Texture not uploaded to GPU!", m_shaderResourceView));
		assert(("Error: Sampler state not uploaded to GPU!", m_samplerState));

		deviceContext->PSSetSamplers(0, 1, &m_samplerState);
		deviceContext->PSSetShaderResources(0, 1, &m_shaderResourceView);
	}

	unsigned int Texture2D::GetWidth()
	{
		return m_width;
	}

	unsigned int Texture2D::GetHeight()
	{
		return m_height;
	}

	const std::vector<char>& Texture2D::GetData() {
		return m_data;
	}
}