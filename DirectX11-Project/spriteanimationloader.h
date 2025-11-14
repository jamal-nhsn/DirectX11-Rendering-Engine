#pragma once

/*======
INCLUDES
======*/
#include <d3d11.h>
#include <stdio.h>
#include <vector>
#include <tuple>

#include "texturemanager.h"
#include "spriteanimationframe.h"

class SpriteAnimationLoader
{
public:
	SpriteAnimationLoader();
	SpriteAnimationLoader(const SpriteAnimationLoader& other);
	~SpriteAnimationLoader();

	std::vector<SpriteAnimationFrame>* LoadSpriteAnimation(const char* filePath, TextureManager* textureManager);


private:
	void LoadFrame(std::vector<SpriteAnimationFrame>* spriteAnimation, FILE* filePtr, TextureManager* textureManager);

	void LoadCount(char* data, size_t& count);
	void LoadSlide(char* data, std::tuple<int, int>& slide);
	void LoadTexture(char* data, TextureManager* textureManager, std::shared_ptr<Engine::Texture2D>& texture);
	void LoadDimensions(char* data, std::tuple<int, int>& dimensions);
	void LoadSourceOrigin(char* data, std::tuple<int, int>& sourceOrigin);
	void LoadDuration(char* data, float& duration);


	static const char* s_delimiters;
};