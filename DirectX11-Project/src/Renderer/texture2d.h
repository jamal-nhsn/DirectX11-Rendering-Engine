#pragma once

#include <d3d11.h>
#include <stdio.h>

namespace Engine
{
	class Texture2D
	{
	public:
		Texture2D(
			ID3D11Device* device,
			ID3D11DeviceContext* deviceContext,
			unsigned int width,
			unsigned int height,
			DXGI_FORMAT format,
			const void* data,
			unsigned int rowPitch,
			ID3D11SamplerState* samplerState
		);

		// Do not allow copying.
		Texture2D(const Texture2D& other) = delete;
		Texture2D& operator=(const Texture2D& other) = delete;
		// Allow moving.
		Texture2D(Texture2D&& other) noexcept;
		Texture2D& operator=(Texture2D&& other) noexcept;

		~Texture2D();

		bool Bind(ID3D11DeviceContext* deviceContext);

		unsigned int GetWidth();
		unsigned int GetHeight();

	private:
		ID3D11Texture2D* m_texture;
		ID3D11ShaderResourceView* m_shaderResourceView;
		ID3D11SamplerState* m_samplerState;

		unsigned int m_width;
		unsigned int m_height;
	};
}
