#pragma once

/*======
INCLUDES
======*/
#include <vector>

#include "direct3d.h"
#include "scene.h"

class CameraSystem
{
public:
	CameraSystem();
	CameraSystem(const CameraSystem&);
	~CameraSystem();

	void Update(Direct3D* direct3d, Scene* scene);
};