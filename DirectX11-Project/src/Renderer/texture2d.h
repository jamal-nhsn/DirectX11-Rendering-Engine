#pragma once

#include "sampler.h"

#include <cassert>
#include <d3d11.h>
#include <stdio.h>
#include <vector>

namespace Engine
{
	class Texture2D
	{
	public:
		Texture2D(const void* data, unsigned int width, unsigned int height, Sampler* sampler);

		// Do not allow copying.
		Texture2D(const Texture2D& other) = delete;
		Texture2D& operator=(const Texture2D& other) = delete;
		// Allow moving.
		Texture2D(Texture2D&& other) noexcept;
		Texture2D& operator=(Texture2D&& other) noexcept;

		~Texture2D();

		void Upload(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
		void Release();

		void Bind(ID3D11DeviceContext* deviceContext);

		unsigned int GetWidth();
		unsigned int GetHeight();
		const std::vector<char>& GetData();

	private:
		ID3D11Texture2D* m_texture;
		ID3D11ShaderResourceView* m_shaderResourceView;
		Sampler* m_sampler;

		std::vector<char> m_data;
		unsigned int m_width;
		unsigned int m_height;
	};
}
