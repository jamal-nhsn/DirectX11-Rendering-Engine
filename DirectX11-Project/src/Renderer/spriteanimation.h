#pragma once

#include "texture2d.h"

namespace Engine
{
	struct SpriteAnimationFrame
	{
		Texture2D* texture;
		int sourceX;
		int sourceY;
		int width;
		int height;
		float duration;

		SpriteAnimationFrame()
			: texture(0)
			, sourceX(0)
			, sourceY(0)
			, width(1)
			, height(1)
			, duration(1.0f)
		{
		};

		SpriteAnimationFrame(Texture2D* texture, float duration = 1.0f)
			: texture(texture)
			, sourceX(0)
			, sourceY(0)
			, width(texture->GetWidth())
			, height(texture->GetHeight())
			, duration(duration)
		{
		};

		SpriteAnimationFrame(Texture2D* texture, int sourceX, int sourceY, int width, int height, float duration = 1.0f)
			: texture(texture)
			, sourceX(sourceX)
			, sourceY(sourceY)
			, width(width)
			, height(height)
			, duration(duration)
		{
		};
	};

	// A sprite animation is just a vector of sprite animation frames.
	using SpriteAnimation = std::vector<SpriteAnimationFrame>;
}