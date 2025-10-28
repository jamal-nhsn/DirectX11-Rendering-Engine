#include "rendersystem.h"
#include "shader.h"

bool RenderSystem::Initialize(ID3D11Device* device)
{
	return m_renderer2D.Initialize(device);
}

void RenderSystem::Update(Direct3D* direct3d, Scene* scene)
{
	direct3d->Clear(0.0f, 0.0f, 0.0f, 1.0f);

	ID3D11DeviceContext* deviceContext = direct3d->GetDeviceContext();

	m_render3DSystem.Update(deviceContext, scene);

	std::vector<Camera2D>* camera2Ds = scene->GetComponents<Camera2D>();
	std::vector<Sprite>* sprites = scene->GetComponents<Sprite>();

	for (Camera2D& camera2D : *camera2Ds)
	{
		m_renderer2D.BeginScene(camera2D);
		for (Sprite& sprite : *sprites) {
			DirectX::XMMATRIX spriteModelMatrix = scene->GetComponent<Transform>(sprite.GetEntityId()).GetModelMatrix();
			DirectX::XMFLOAT4 spriteTint = sprite.GetTint();
			m_renderer2D.SubmitSprite(
				sprite.GetShader(),
				sprite.GetTexture(),
				spriteModelMatrix,
				sprite.GetSourceX(),
				sprite.GetSourceY(),
				sprite.GetWidth(),
				sprite.GetHeight(),
				spriteTint
			);
		}
		m_renderer2D.EndScene(deviceContext);
	}

	direct3d->Render();
}