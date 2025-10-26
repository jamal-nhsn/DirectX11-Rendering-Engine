#pragma once

/*======
INCLUDES
======*/
#include "texture.h"

struct SpriteAnimationFrame
{
	Texture* texture;
	int width;
	int height;
	int sourceX;
	int sourceY;
	float duration;
};