#pragma once

/*======
INCLUDES
======*/
#include <unordered_map>

#include "material.h"
#include "shadermanager.h"

class MaterialManager
{
public:
	MaterialManager();
	MaterialManager(const MaterialManager&);
	~MaterialManager();

	bool Initialize(ShaderManager* shaderManager);
	Material* GetMaterial(const char* materialName);
	void Shutdown();

private:
	std::unordered_map<const char*, Material*> m_materialBank;
};