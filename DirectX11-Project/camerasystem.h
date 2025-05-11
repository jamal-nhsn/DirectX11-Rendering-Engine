#pragma once

/*======
INCLUDES
======*/
#include <vector>

#include "scene.h"

class CameraSystem
{
public:
	CameraSystem();
	CameraSystem(const CameraSystem&);
	~CameraSystem();

	void Update(Scene* scene);
};