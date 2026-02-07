#pragma once

/*======
INCLUDES
======*/
#include <d3d11.h>
#include <stdio.h>
#include <vector>
#include <tuple>

#include "src/Resources/resourcemanager.h"

#include "texturemanager.h"
#include "spriteanimationframe.h"

class SpriteAnimationLoader
{
public:
	SpriteAnimationLoader();
	SpriteAnimationLoader(const SpriteAnimationLoader& other);
	~SpriteAnimationLoader();

	std::vector<SpriteAnimationFrame>* LoadSpriteAnimation(const char* filePath, Engine::ResourceManager* resourceManager);


private:
	void LoadFrame(std::vector<SpriteAnimationFrame>* spriteAnimation, FILE* filePtr, Engine::ResourceManager* resourceManager);

	void LoadCount(char* data, size_t& count);
	void LoadSlide(char* data, std::tuple<int, int>& slide);
	void LoadTexture(char* data, Engine::ResourceManager* resourceManager, Engine::Texture2D*& texture);
	void LoadDimensions(char* data, std::tuple<int, int>& dimensions);
	void LoadSourceOrigin(char* data, std::tuple<int, int>& sourceOrigin);
	void LoadDuration(char* data, float& duration);


	static const char* s_delimiters;
};