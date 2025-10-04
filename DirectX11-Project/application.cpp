#include "application.h"

Application::Application()
{
	m_direct3d = 0;

	m_shaderManager = 0;
	m_meshManager = 0;
	m_textureManager = 0;

	m_transformSystem = 0;
	m_cameraSystem = 0;
	m_renderSystem = 0;

	m_scene = 0;
}

Application::Application(const Application& other)
{
}

Application::~Application()
{
}

bool Application::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool success = false;

	// Create and initialize the Direct3D object.
	m_direct3d = new Direct3D;
	success = m_direct3d->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!success) {
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return success;
	}

	// Create and initialize the ShaderManager object.
	m_shaderManager = new ShaderManager;
	success = m_shaderManager->Initialize(m_direct3d->GetDevice(), hwnd);
	if (!success) {
		MessageBox(hwnd, L"Could not initialize shaders", L"Error", MB_OK);
		return success;
	}

	// Create and initialize the MeshManager object.
	m_meshManager = new MeshManager;
	success = m_meshManager->Initialize(m_direct3d->GetDevice());
	if (!success) {
		MessageBox(hwnd, L"Could not initialize meshes", L"Error", MB_OK);
		return success;
	}

	// Create and initialize the TextureManager object.
	m_textureManager = new TextureManager;
	success = m_textureManager->Initialize(m_direct3d->GetDevice(), m_direct3d->GetDeviceContext());
	if (!success) {
		MessageBox(hwnd, L"Could not initalize textures", L"Error", MB_OK);
		return success;
	}

	// Create the System objects.
	m_transformSystem = new TransformSystem;
	m_cameraSystem = new CameraSystem;
	m_renderSystem = new RenderSystem;

	// Create and initialize the Scene object.
	m_scene = new Scene;
	m_scene->Initialize(screenWidth, screenHeight);
	m_scene->SetAmbientLight(DirectX::XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f));

	// Get camera entity from scene.
	int camera = 0;
	m_scene->GetComponent<Transform>(camera).SetGlobalPosition(0.0f, 0.0f, -3.0f);

	// Add lights to scene.
	int lightParent = m_scene->CreateEntity();
	m_scene->AddComponent<Transform>(lightParent);

	int spotLight1 = m_scene->CreateEntity();
	m_scene->AddComponent<Transform>(spotLight1);
	m_scene->GetComponent<Transform>(spotLight1).SetGlobalPosition(0.0f, 0.0f, -4.0f);
	m_scene->AddComponent<Light>(spotLight1);
	m_scene->GetComponent<Light>(spotLight1).SetType(SPOT_LIGHT_TYPE);
	m_scene->GetComponent<Light>(spotLight1).SetColor(DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));

	int pointLight1 = m_scene->CreateEntity();
	m_scene->AddComponent<Transform>(pointLight1);
	m_scene->GetComponent<Transform>(pointLight1).SetGlobalRotation(180.0f, 0.0f, 1.0f, 0.0f);
	m_scene->GetComponent<Transform>(pointLight1).SetGlobalPosition(0.0f, 0.0f, 1.125f);
	m_scene->AddComponent<Light>(pointLight1);
	m_scene->GetComponent<Light>(pointLight1).SetType(POINT_LIGHT_TYPE);
	m_scene->GetComponent<Light>(pointLight1).SetColor(DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));

	m_scene->GetComponent<Transform>(lightParent).AddChild(spotLight1, m_scene);
	m_scene->GetComponent<Transform>(lightParent).AddChild(pointLight1, m_scene);

	// Add sphere to scene.
	int sphere1 = m_scene->CreateEntity();
	m_scene->AddComponent<Transform>(sphere1);
	m_scene->AddComponent<Model>(sphere1);
	Model& model1 = m_scene->GetComponent<Model>(sphere1);
	model1.SetMesh(m_meshManager->GetMesh("sphere"));
	model1.SetBaseShader(m_shaderManager->GetShader<DefaultBaseShader>());
	model1.SetLightShader(m_shaderManager->GetShader<DefaultLightShader>());
	model1.SetTexture(m_textureManager->GetTexture("stoneWall"));
	model1.SetShininess(4);
	
	return success;
}

void Application::Shutdown()
{
	// Release the Direct3D object.
	if (m_direct3d) {
		m_direct3d->Shutdown();
		delete m_direct3d;
		m_direct3d = 0;
	}

	// Release the ShaderManager object.
	if (m_shaderManager) {
		m_shaderManager->Shutdown();
		delete m_shaderManager;
		m_shaderManager = 0;
	}

	// Release the MeshManager object.
	if (m_meshManager) {
		m_meshManager->Shutdown();
		delete m_meshManager;
		m_meshManager = 0;
	}

	// Release the TextureManager object.
	if (m_textureManager) {
		m_textureManager->Shutdown();
		delete m_textureManager;
		m_textureManager = 0;
	}

	// Release the System objects.
	if (m_transformSystem) {
		delete m_transformSystem;
		m_transformSystem = 0;
	}
	if (m_cameraSystem) {
		delete m_cameraSystem;
		m_cameraSystem = 0;
	}
	if (m_renderSystem) {
		delete m_renderSystem;
		m_renderSystem = 0;
	}

	// Release the Scene object.
	if (m_scene) {
		delete m_scene;
		m_scene = 0;
	}
}

bool Application::Tick(float dt)
{
	bool success = true;
	
	m_transformSystem->Update(m_scene);
	m_cameraSystem->Update(m_scene);
	m_renderSystem->Update(m_direct3d, m_scene);

	Transform& transform1 = m_scene->GetComponent<Transform>(1);
	transform1.SetLocalRotation(
		DirectX::XMQuaternionMultiply(
			transform1.GetLocalRotation(),
			DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), DirectX::XMConvertToRadians(36.0f) * dt)
		)
	);
	
	return success;
}