#pragma once

/*======
INCLUDES
======*/
#include <unordered_map>
#include <filesystem>
#include <string>

#include "src/Resources/resourcemanager.h"

#include "spriteanimationloader.h"

class SpriteAnimationManager
{
public:
    SpriteAnimationManager();
    SpriteAnimationManager(const SpriteAnimationManager&);
    ~SpriteAnimationManager();

    bool Initialize(Engine::ResourceManager* resourceManager);
    const std::vector<SpriteAnimationFrame>* GetSpriteAnimation(std::string spriteAnimationName);
    void Shutdown();

private:
    bool LoadSpriteAnimation(Engine::ResourceManager* resourceManager, const std::filesystem::path& spriteAnimationPath);

private:
    std::unordered_map<std::string, const std::vector<SpriteAnimationFrame>*> m_spriteAnimationBank;
};