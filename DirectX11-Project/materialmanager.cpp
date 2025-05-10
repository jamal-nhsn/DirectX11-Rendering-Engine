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

bool MaterialManager::Initialize(ShaderManager* shaderManager)
{
	bool success = false;

	Material* colorMaterial = new Material();
	success = colorMaterial->Initialize(shaderManager->GetShader<ColorShader>());

	if (!success) {
		delete colorMaterial;
		return success;
	}

	m_materialBank["colorMaterial"] = colorMaterial;

	return success;
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