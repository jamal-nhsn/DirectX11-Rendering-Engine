#pragma once

/*======
INCLUDES
======*/
#include "texture.h"

struct SpriteAnimationFrame
{
	Texture* texture;
	int sourceX;
	int sourceY;
	int width;
	int height;
	float duration;
};