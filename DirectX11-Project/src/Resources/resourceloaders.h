#pragma once

#include "../Renderer/mesh.h"
#include "../Renderer/texture2d.h"

#include <memory>
#include <filesystem>

namespace Engine
{
	std::unique_ptr<Mesh> LoadMeshOBJ(const std::filesystem::path& filepath, ID3D11Device* device);
	std::unique_ptr<Mesh> LoadMeshBinary(const std::filesystem::path& filepath, ID3D11Device* device);

	D3D11_SAMPLER_DESC LoadTextureMeta(const std::filesystem::path& filepath);
	std::unique_ptr<Texture2D> LoadTGA(const std::filesystem::path& filepath, ID3D11Device* device, ID3D11DeviceContext* deviceContext, ID3D11SamplerState* samplerState);
}