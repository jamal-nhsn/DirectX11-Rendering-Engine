#pragma once

/*======
INCLUDES
======*/
#include "scene.h"

class SpriteAnimatorSystem
{
public:
	SpriteAnimatorSystem();
	SpriteAnimatorSystem(const SpriteAnimatorSystem&);
	~SpriteAnimatorSystem();

	void Update(Scene* scene, float dt);
};