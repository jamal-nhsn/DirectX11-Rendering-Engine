#pragma once

/*======
INCLUDES
======*/
#include "scene.h"
#include "direct3d.h"
#include "render3dsystem.h"
#include "renderer2d.h"

class RenderSystem
{
public:
	RenderSystem() = default;
	RenderSystem(const RenderSystem&) = default;
	~RenderSystem() = default;

	bool Initialize(ID3D11Device* device);
	void Update(Direct3D* direct3d, Scene* scene);

private:
	Renderer2D m_renderer2D;
	Render3DSystem m_render3DSystem;
};