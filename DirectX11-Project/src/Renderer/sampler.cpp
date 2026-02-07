#include "pch.h"
#include "sampler.h"

namespace Engine
{
	Sampler::Sampler(D3D11_SAMPLER_DESC samplerDesc)
		: m_samplerDesc(samplerDesc)
	{
	}

	Sampler::Sampler(Sampler&& other) noexcept
	{
		// Release what we currently have.
		Release();

		// Copy other.
		m_samplerDesc = other.m_samplerDesc;
		m_samplerState = other.m_samplerState;

		// Wipe other.
		other.m_samplerDesc = D3D11_SAMPLER_DESC{};
		other.m_samplerState = 0;
	}

	Sampler& Sampler::operator=(Sampler&& other) noexcept
	{
		// Release what we currently have.
		Release();

		// Copy other.
		m_samplerDesc = other.m_samplerDesc;
		m_samplerState = other.m_samplerState;

		// Wipe other.
		other.m_samplerDesc = D3D11_SAMPLER_DESC{};
		other.m_samplerState = 0;

		return *this;
	}

	Sampler::~Sampler()
	{
		Release();
	}

	void Sampler::Upload(ID3D11Device* device)
	{
		// Already uploaded.
		if (m_samplerState) {
			return;
		}

		HRESULT result = device->CreateSamplerState(&m_samplerDesc, &m_samplerState);

		assert(("Error: Could not upload Sampler to the GPU!", !FAILED(result)));
	}

	void Sampler::Release()
	{
		if (m_samplerState) {
			m_samplerState->Release();
			m_samplerState = 0;
		}
	}

	void Sampler::Bind(ID3D11DeviceContext* deviceContext)
	{
		assert(("Error: Sampler not uploaded to GPU!", m_samplerState));

		deviceContext->PSSetSamplers(0, 1, &m_samplerState);
	}

	const D3D11_SAMPLER_DESC& Sampler::GetDesc()
	{
		return m_samplerDesc;
	}
};