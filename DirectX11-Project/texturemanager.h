#pragma once

/*======
INCLUDES
======*/
#include <unordered_map>

#include "targaloader.h"
#include "texturemetaloader.h"

class TextureManager
{
public:
	TextureManager();
	TextureManager(const TextureManager&);
	~TextureManager();

	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	Texture* GetTexture(const char* textureName);
	void Shutdown();

private:
    struct SamplerDescEqual
    {
        bool operator()(const D3D11_SAMPLER_DESC& a, const D3D11_SAMPLER_DESC& b) const noexcept
        {
            return a.Filter == b.Filter &&
                a.AddressU == b.AddressU &&
                a.AddressV == b.AddressV &&
                a.AddressW == b.AddressW &&
                a.MipLODBias == b.MipLODBias &&
                a.MaxAnisotropy == b.MaxAnisotropy &&
                a.ComparisonFunc == b.ComparisonFunc &&
                a.MinLOD == b.MinLOD &&
                a.MaxLOD == b.MaxLOD &&
                a.BorderColor[0] == b.BorderColor[0] &&
                a.BorderColor[1] == b.BorderColor[1] &&
                a.BorderColor[2] == b.BorderColor[2] &&
                a.BorderColor[3] == b.BorderColor[3];
        }
    };

    struct SamplerDescHash
    {
        size_t operator()(const D3D11_SAMPLER_DESC& desc) const noexcept
        {
            auto hash_combine = [](size_t& seed, auto v)
                {
                    std::hash<std::decay_t<decltype(v)>> h;
                    seed ^= h(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
                };

            size_t h = 0;
            hash_combine(h, desc.Filter);
            hash_combine(h, desc.AddressU);
            hash_combine(h, desc.AddressV);
            hash_combine(h, desc.AddressW);
            hash_combine(h, desc.MipLODBias);
            hash_combine(h, desc.MaxAnisotropy);
            hash_combine(h, desc.ComparisonFunc);
            hash_combine(h, desc.MinLOD);
            hash_combine(h, desc.MaxLOD);
            hash_combine(h, desc.BorderColor[0]);
            hash_combine(h, desc.BorderColor[1]);
            hash_combine(h, desc.BorderColor[2]);
            hash_combine(h, desc.BorderColor[3]);
            return h;
        }
    };

private:
    std::unordered_map<const char*, Texture*> m_textureBank;
    std::unordered_map<D3D11_SAMPLER_DESC, ID3D11SamplerState*, SamplerDescHash, SamplerDescEqual> m_samplerBank;
};