#include "materialmanager.h"

MaterialManager::MaterialManager()
{
}

MaterialManager::MaterialManager(const MaterialManager& other)
{
}

MaterialManager::~MaterialManager()
{
}

void MaterialManager::Initialize(ShaderManager* shaderManager, TextureManager* textureManager)
{
	Material* color = new Material();
	color->SetShader(shaderManager->GetShader<ColorShader>());
	color->SetTexture(0);
	m_materialBank["color"] = color;

	Material* stoneWall = new Material();
	stoneWall->SetShader(shaderManager->GetShader<TextureShader>());
	stoneWall->SetTexture(textureManager->GetTexture("stoneWall"));
	m_materialBank["stoneWall"] = stoneWall;
}

Material* MaterialManager::GetMaterial(const char* materialName)
{
	return m_materialBank[materialName];
}

void MaterialManager::Shutdown()
{
	for (auto& entry : m_materialBank) {
		delete entry.second;
	}
}