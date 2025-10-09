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

void RenderSystem::Update(Direct3D* direct3d, Scene* scene)
{
	std::vector<Camera3D>* camera3Ds = scene->GetComponents<Camera3D>();
	std::vector<Model>* models       = scene->GetComponents<Model>();
	std::vector<Light>* lights       = scene->GetComponents<Light>();

	ID3D11DeviceContext* deviceContext = direct3d->GetDeviceContext();

	direct3d->Clear(0.0f, 0.0f, 0.0f, 1.0f);

	DirectX::XMFLOAT4 ambientLight = scene->GetAmbientLight();

	for (Camera3D& camera : (*camera3Ds)) {

		Transform& cameraTransform = scene->GetComponent<Transform>(0);
		int renderMask = camera.GetRenderMask();

		/*-----------BASE-PASS-----------*/
		for (Model& model : (*models)) {

			int entity = model.GetEntityId();

			if (!scene->HasComponent<Renderer>(entity)) {
				continue;
			}
			int renderLayer = scene->GetComponent<Renderer>(entity).GetLayer();

			if (!(renderLayer & renderMask)) {
				continue;
			}

			Mesh* mesh = model.GetMesh();
			Texture* texture = model.GetTexture();
			Shader* shader = model.GetBaseShader();

			if (shader == 0) {
				continue;
			}

			mesh->Bind(deviceContext);
			Transform& modelTransform = scene->GetComponent<Transform>(entity);

			shader->Bind(deviceContext, camera, model, modelTransform, ambientLight);
			deviceContext->DrawIndexed(mesh->GetIndexCount(), 0, 0);
		}

		/*----------LIGHT-PASS----------*/
		for (Model& model : (*models)) {

			int entity = model.GetEntityId();

			if (!scene->HasComponent<Renderer>(entity)) {
				continue;
			}
			int renderLayer = scene->GetComponent<Renderer>(entity).GetLayer();

			if (!(renderLayer & renderMask)) {
				continue;
			}

			Mesh* mesh = model.GetMesh();
			Texture* texture = model.GetTexture();
			Shader* shader = model.GetLightShader();

			if (shader == 0) {
				continue;
			}

			mesh->Bind(deviceContext);
			Transform& modelTransform = scene->GetComponent<Transform>(entity);

			for (Light& light : (*lights)) {
				Transform& lightTransform = scene->GetComponent<Transform>(light.GetEntityId());

				shader->Bind(deviceContext, camera, cameraTransform, model, modelTransform, light, lightTransform);
				deviceContext->DrawIndexed(mesh->GetIndexCount(), 0, 0);
			}
		}
	}

	direct3d->Render();
}