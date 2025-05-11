#pragma once

/*======
INCLUDES
======*/
#include <vector>

#include "scene.h"
#include "direct3d.h"

class RenderSystem
{
public:
	RenderSystem();
	RenderSystem(const RenderSystem&);
	~RenderSystem();

	void Update(Direct3D* direct3d, Scene* scene);
};