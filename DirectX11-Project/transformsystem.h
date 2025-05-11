#pragma once

/*======
INCLUDES
======*/
#include <vector>

#include "scene.h"

class TransformSystem
{
public:
	TransformSystem();
	TransformSystem(const TransformSystem&);
	~TransformSystem();

	void Update(Scene* scene);
};