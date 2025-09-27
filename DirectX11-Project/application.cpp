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
	m_cameraSystem    = new CameraSystem;
	m_renderSystem    = new RenderSystem;

	// Create and initialize the Scene object.
	m_scene = new Scene;
	m_scene->Initialize(screenWidth, screenHeight);

	// Get camera entity from scene.
	int camera = 0;
	m_scene->GetComponent<Transform>(camera).SetGlobalPosition(0.0f, 2.0f, -2.0f);
	m_scene->GetComponent<Transform>(camera).SetGlobalRotation(45.0f, 1.0f, 0.0f, 0.0f);

	// Add direction light to scene.
	int dirLight1 = m_scene->CreateEntity();
	m_scene->AddComponent<Transform>(dirLight1);
	m_scene->AddComponent<Light>(dirLight1);
	m_scene->GetComponent<Transform>(dirLight1).SetGlobalRotation(45.0f, 1.0f, 0.0f, 0.0f);
	m_scene->GetComponent<Light>(dirLight1).SetColor(DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

	int dirLight2 = m_scene->CreateEntity();
	m_scene->AddComponent<Transform>(dirLight2);
	m_scene->AddComponent<Light>(dirLight2);
	m_scene->GetComponent<Transform>(dirLight2).SetGlobalRotation(180.0f, 0.0f, 1.0f, 0.0f);
	m_scene->GetComponent<Light>(dirLight2).SetColor(DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));

	// Add cube to scene.
	int cube1 = m_scene->CreateEntity();
	m_scene->AddComponent<Transform>(cube1);
	m_scene->AddComponent<Model>(cube1);
	Model& model1 = m_scene->GetComponent<Model>(cube1);
	model1.SetMesh(m_meshManager->GetMesh("cube"));
	model1.SetShader(m_shaderManager->GetShader<DirLightShader>());
	model1.SetTexture(m_textureManager->GetTexture("stoneWall"));
	m_scene->GetComponent<Transform>(cube1).SetGlobalScale(1.0f, 1.0f, 1.0f);

	// Add another cube to scene.
	int cube2 = m_scene->CreateEntity();
	m_scene->AddComponent<Transform>(cube2);
	m_scene->AddComponent<Model>(cube2);
	Model& model2 = m_scene->GetComponent<Model>(cube2);
	model2.SetMesh(m_meshManager->GetMesh("cube"));
	model2.SetShader(m_shaderManager->GetShader<DirLightShader>());
	model2.SetTexture(m_textureManager->GetTexture("stoneWall"));
	m_scene->GetComponent<Transform>(cube2).SetLocalPosition(-1.5f, 0.0f, 0.0f);
	m_scene->GetComponent<Transform>(cube2).SetGlobalScale(0.5f, 0.5f, 0.5f);

	// Make cube2 a child of cube1.
	m_scene->GetComponent<Transform>(cube1).AddChild(cube2, m_scene);
	
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

	Transform& transform2 = m_scene->GetComponent<Transform>(3);
	transform2.SetLocalRotation(
		DirectX::XMQuaternionMultiply(
			transform2.GetLocalRotation(),
			DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), DirectX::XMConvertToRadians(36.0f) * dt)
		)
	);

	Transform& transform3 = m_scene->GetComponent<Transform>(4);
	transform3.SetLocalRotation(
		DirectX::XMQuaternionMultiply(
			transform3.GetLocalRotation(),
			DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(1.0f, 0.0f, 1.0f, 0.0f), DirectX::XMConvertToRadians(36.0f) * dt)
		)
	);
	
	return success;
}