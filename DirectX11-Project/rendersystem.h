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
	/* TEXT TEST DELETE TEXTDATA + MODEL MATRIX SOON*/
	void Update(Direct3D* direct3d, Scene* scene, const TextData& textData, const DirectX::XMMATRIX& modelMatrix);

private:
	Renderer2D m_renderer2D;
	Render3DSystem m_render3DSystem;
};