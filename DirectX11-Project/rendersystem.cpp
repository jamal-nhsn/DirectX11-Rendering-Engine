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

	SpriteData spriteData = m_renderer2D.BuildSprite();

	for (Camera2D& camera2D : *camera2Ds)
	{
		m_renderer2D.BeginScene(camera2D);
		for (Sprite& sprite : *sprites) {

			const DirectX::XMMATRIX* modelMatrix = &scene->GetComponent<Transform>(sprite.GetEntityId()).GetModelMatrix();

			spriteData // Build the sprite.
				.WithModelMatrix(modelMatrix)
				.WithSourceOrigin(sprite.GetSourceX(), sprite.GetSourceY())
				.WithSourceDimensions(sprite.GetWidth(), sprite.GetHeight())
				.WithTint(sprite.GetTint())
				.Submit(sprite.GetShader(), sprite.GetTexture());
		}
		m_renderer2D.EndScene(deviceContext);
	}

	direct3d->Render();
}