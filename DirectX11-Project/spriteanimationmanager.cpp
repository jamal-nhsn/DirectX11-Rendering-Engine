#include "pch.h"
#include "spriteanimationmanager.h"

SpriteAnimationManager::SpriteAnimationManager()
{
}

SpriteAnimationManager::SpriteAnimationManager(const SpriteAnimationManager& other)
{
}

SpriteAnimationManager::~SpriteAnimationManager()
{
}

bool SpriteAnimationManager::Initialize(TextureManager* textureManager)
{
	bool success = false;
	for (const auto& file : std::filesystem::directory_iterator("Resources/SpriteAnimations")) {
		// Get the path to the sprite animation itself.
		const auto& spriteAnimationPath = file.path();

		// Load the texture.
		success = LoadSpriteAnimation(textureManager, spriteAnimationPath);

		if (!success) {
			return success;
		}
	}

	return success;
}

bool SpriteAnimationManager::LoadSpriteAnimation(TextureManager* textureManager, const std::filesystem::path& spriteAnimationPath)
{
	std::vector<SpriteAnimationFrame>* spriteAnimation = 0;
	if (spriteAnimationPath.extension().compare(".spriteanimation") == 0) {
		// Create texture from targa image.
		SpriteAnimationLoader spriteAnimationLoader;
		spriteAnimation = spriteAnimationLoader.LoadSpriteAnimation(spriteAnimationPath.string().c_str(), textureManager);
	}
	else {
		// Not a spriteanimation file, ignore it.
		return true;
	}
	if (!spriteAnimation) {
		return false;
	}

	m_spriteAnimationBank[spriteAnimationPath.stem().string()] = spriteAnimation;

	return true;
}


const std::vector<SpriteAnimationFrame>* SpriteAnimationManager::GetSpriteAnimation(std::string spriteAnimationName)
{
	return m_spriteAnimationBank[spriteAnimationName];
}

void SpriteAnimationManager::Shutdown()
{
	for (auto& entry : m_spriteAnimationBank) {
		delete entry.second;
	}
	m_spriteAnimationBank.clear();
}