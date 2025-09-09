#include "rendersystem.h"

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
		Texture* texture     = model.GetTexture();
		Shader* shader       = model.GetShader();
		Transform& transform = scene->GetComponent<Transform>(model.GetEntityId());

		Camera& camera = scene->GetComponent<Camera>(0); // Camera is the first entity always.

		mesh->Bind(deviceContext);
		shader->Bind(deviceContext, texture, transform, camera);

		deviceContext->DrawIndexed(mesh->GetIndexCount(), 0, 0);
	}

	direct3d->Render();
}