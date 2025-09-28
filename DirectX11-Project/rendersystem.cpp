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
	std::vector<Model>* models = scene->GetComponents<Model>();
	std::vector<Light>* lights = scene->GetComponents<Light>();
	ID3D11DeviceContext* deviceContext = direct3d->GetDeviceContext();

	Camera& camera = scene->GetComponent<Camera>(0);

	direct3d->Clear(0.0f, 0.0f, 0.0f, 1.0f);

	/*---------AMBIENT-PASS---------*/
	for (Model& model : (*models)) {
		Mesh* mesh = model.GetMesh();
		Texture* texture = model.GetTexture();
		Shader* shader = model.GetShader();

		if (shader == 0) {
			continue;
		}

		mesh->Bind(deviceContext);
		Transform& modelTransform = scene->GetComponent<Transform>(model.GetEntityId());

		bool isLit = shader->IsLit();
		for (Light& light : (*lights)) {
			Transform& lightTransform = scene->GetComponent<Transform>(light.GetEntityId());

			shader->Bind(deviceContext, camera, model, modelTransform, light, lightTransform);
			deviceContext->DrawIndexed(mesh->GetIndexCount(), 0, 0);

			if (!isLit) break;
		}
	}


	direct3d->Render();
}