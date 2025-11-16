#include "rendersystem.h"
#include "shader.h"

bool RenderSystem::Initialize(ID3D11Device* device)
{
	return m_renderer2D.Initialize(device);
}

void RenderSystem::Update(Direct3D* direct3d, Scene* scene)
{
	direct3d->Clear(0.0f, 0.0f, 0.0f, 1.0f);

	ID3D11Device* device = direct3d->GetDevice();
	ID3D11DeviceContext* deviceContext = direct3d->GetDeviceContext();

	m_render3DSystem.Update(device, deviceContext, scene);

	std::vector<Camera2D>* camera2Ds = scene->GetComponents<Camera2D>();
	std::vector<Sprite>* sprites = scene->GetComponents<Sprite>();
	std::vector<Text>* texts = scene->GetComponents<Text>();

	for (Camera2D& camera2D : *camera2Ds)
	{
		m_renderer2D.BeginScene(camera2D);
		for (Sprite& sprite : *sprites) {
			m_renderer2D.SubmitSprite(
				sprite.GetSpriteData(),
				scene->GetComponent<Transform>(sprite.GetEntityId()).GetModelMatrix()
			);
		}
		for (Text& text : *texts) {
			m_renderer2D.SubmitText(
				text.GetTextData(),
				scene->GetComponent<Transform>(text.GetEntityId()).GetModelMatrix()
			);
		}
		m_renderer2D.EndScene(device, deviceContext);
	}

	direct3d->Render();
}