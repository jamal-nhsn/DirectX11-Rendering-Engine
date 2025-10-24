#pragma once

/*======
INCLUDES
======*/
#include <vector>

#include "scene.h"
#include "direct3d.h"
#include "render2dsystem.h"
#include "render3dsystem.h"

class RenderSystem
{
public:
	RenderSystem();
	RenderSystem(const RenderSystem&);
	~RenderSystem();

	void Initialize(ID3D11Device* device);
	void Update(Direct3D* direct3d, Scene* scene);

private:
	Render2DSystem m_render2DSystem;
	Render3DSystem m_render3DSystem;
};