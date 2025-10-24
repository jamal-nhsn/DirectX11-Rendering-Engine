#include "rendersystem.h"
#include "shader.h"

RenderSystem::RenderSystem()
{
};

RenderSystem::RenderSystem(const RenderSystem& other)
{
}

RenderSystem::~RenderSystem()
{
}

void RenderSystem::Initialize(ID3D11Device* device)
{
	m_render2DSystem.Initialize(device);
}

void RenderSystem::Update(Direct3D* direct3d, Scene* scene)
{
	direct3d->Clear(0.0f, 0.0f, 0.0f, 1.0f);

	ID3D11DeviceContext* deviceContext = direct3d->GetDeviceContext();

	m_render3DSystem.Update(deviceContext, scene);
	m_render2DSystem.Update(deviceContext, scene);

	direct3d->Render();
}