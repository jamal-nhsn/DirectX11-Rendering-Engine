#pragma once

/*======
INCLUDES
======*/
#include "src/Renderer/texture2d.h"

#include <memory>

struct SpriteAnimationFrame
{
	Engine::Texture2D* texture;
	int sourceX;
	int sourceY;
	int width;
	int height;
	float duration;
};