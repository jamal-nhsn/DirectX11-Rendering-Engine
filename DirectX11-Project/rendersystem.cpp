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

			Mesh* mesh = model.GetMesh();
			Texture* texture = model.GetTexture();
			Shader* shader = model.GetBaseShader();

			if (shader == 0) {
				continue;
			}

			ID3D11SamplerState* samplerState = texture->GetSamplerState();
			ID3D11ShaderResourceView* shaderResourceView = texture->GetTexture2D();

			direct3d->GetDeviceContext()->PSSetSamplers(0, 1, &samplerState);
			direct3d->GetDeviceContext()->PSSetShaderResources(0, 1, &shaderResourceView);

			mesh->Bind(deviceContext);
			DirectX::XMMATRIX modelMatrix = scene->GetComponent<Transform>(entity).GetModelMatrix();

			shader->Bind(
				deviceContext,
				modelMatrix,
				viewMatrix,
				projectionMatrix,
				ambientLight
			);

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

			ID3D11SamplerState* samplerState = texture->GetSamplerState();
			ID3D11ShaderResourceView* shaderResourceView = texture->GetTexture2D();

			direct3d->GetDeviceContext()->PSSetSamplers(0, 1, &samplerState);
			direct3d->GetDeviceContext()->PSSetShaderResources(0, 1, &shaderResourceView);

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

				deviceContext->DrawIndexed(mesh->GetIndexCount(), 0, 0);
			}
		}
	}

	direct3d->Render();
}