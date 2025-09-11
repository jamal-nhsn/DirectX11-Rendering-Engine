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
	ID3D11DeviceContext* deviceContext = direct3d->GetDeviceContext();

	direct3d->Clear(0.0f, 0.0f, 0.0f, 1.0f);

	for (Model& model : (*models)) {
		Mesh* mesh           = model.GetMesh();
		Material* material   = model.GetMaterial();
		Transform& transform = scene->GetComponent<Transform>(model.GetEntityId());

		Camera& camera = scene->GetComponent<Camera>(0); // Camera is the first entity always.

		if (material != 0) {
			mesh->Bind(deviceContext);
			material->GetShader()->Bind(deviceContext, camera, transform, material);
		}

		deviceContext->DrawIndexed(mesh->GetIndexCount(), 0, 0);
	}

	direct3d->Render();
}