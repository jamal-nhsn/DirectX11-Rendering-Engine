#include "render3dSystem.h"
#include "shader.h"
#include "src/Renderer/texture2d.h"

Render3DSystem::Render3DSystem()
{
};

Render3DSystem::Render3DSystem(const Render3DSystem& other)
{
}

Render3DSystem::~Render3DSystem()
{
}

void Render3DSystem::Update(ID3D11Device* device, ID3D11DeviceContext* deviceContext, Scene* scene)
{
	std::vector<Camera3D>* camera3Ds = scene->GetComponents<Camera3D>();
	std::vector<Model>* models       = scene->GetComponents<Model>();
	std::vector<Light>* lights       = scene->GetComponents<Light>();


	DirectX::XMFLOAT4 ambientLight = scene->GetAmbientLight();

	for (Camera3D& camera : (*camera3Ds)) {

		DirectX::XMMATRIX viewMatrix = camera.GetViewMatrix();
		DirectX::XMMATRIX projectionMatrix = camera.GetProjectionMatrix();
		DirectX::XMFLOAT3 cameraPosition = scene->GetComponent<Transform>(camera.GetEntityId()).GetGlobalPosition();

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

			Engine::Mesh* mesh = model.GetMesh();
			Engine::Texture2D* texture = model.GetTexture();
			Shader* shader = model.GetBaseShader();

			if (shader == 0) {
				continue;
			}

			// Upload texture and mesh to device.
			texture->Upload(device, deviceContext);
			mesh->Upload(device);

			texture->Bind(deviceContext);

			mesh->Bind(deviceContext);
			DirectX::XMMATRIX modelMatrix = scene->GetComponent<Transform>(entity).GetModelMatrix();

			shader->Bind(
				deviceContext,
				modelMatrix,
				viewMatrix,
				projectionMatrix,
				ambientLight
			);

			deviceContext->DrawIndexed(mesh->GetIBO().GetIndexCount(), 0, 0);
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

			Engine::Mesh* mesh = model.GetMesh();
			Engine::Texture2D* texture = model.GetTexture();
			Shader* shader = model.GetLightShader();

			if (shader == 0) {
				continue;
			}

			texture->Bind(deviceContext);

			mesh->Bind(deviceContext);
			DirectX::XMMATRIX modelMatrix = scene->GetComponent<Transform>(entity).GetModelMatrix();

			DirectX::XMFLOAT4 specularTint = model.GetSpecularTint();
			float shininess = model.GetShininess();

			for (Light& light : (*lights)) {
			
				LightData lightData;
				Transform& lightTransform = scene->GetComponent<Transform>(light.GetEntityId());
				light.FillLightData(&lightData, lightTransform.GetGlobalPosition(), lightTransform.GetForward());

				shader->Bind(
					deviceContext,
					modelMatrix,
					viewMatrix,
					projectionMatrix,
					cameraPosition,
					specularTint,
					shininess,
					lightData
				);

				deviceContext->DrawIndexed(mesh->GetIBO().GetIndexCount(), 0, 0);
			}
		}
	}
}