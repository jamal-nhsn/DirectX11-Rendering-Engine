#include "application.h"

Application::Application()
{
	m_direct3d = 0;

	m_shaderManager = 0;
	m_meshManager = 0;
	m_materialManager = 0;

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

	// Create and initialize the MaterialManager object.
	m_materialManager = new MaterialManager;
	success = m_materialManager->Initialize();
	if (!success) {
		MessageBox(hwnd, L"Could not initalize materials", L"Error", MB_OK);
		return success;
	}

	// Create the System objects.
	m_transformSystem = new TransformSystem;
	m_cameraSystem    = new CameraSystem;
	m_renderSystem    = new RenderSystem;

	// Create and initialize the Scene object.
	m_scene = new Scene;
	m_scene->Initialize(screenWidth, screenHeight);

	// Add triangle to scene.
	int triangle = m_scene->CreateEntity();
	m_scene->AddComponent<Transform>(triangle);
	m_scene->AddComponent<Model>(triangle);
	Model& model = m_scene->GetComponent<Model>(triangle);
	model.SetMesh(m_meshManager->GetMesh("triangle"));
	model.SetShader(m_shaderManager->GetShader<ColorShader>());
	model.SetMaterial(m_materialManager->GetMaterial("colorMaterial"));

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

	// Release the MaterialManager object.
	if (m_materialManager) {
		m_materialManager->Shutdown();
		delete m_materialManager;
		m_materialManager = 0;
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

	return success;
}