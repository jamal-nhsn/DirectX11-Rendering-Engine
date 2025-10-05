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
	m_scene->SetAmbientLight(DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 0.25f));

	// Get camera entity from scene.
	int camera = 0;
	m_scene->GetComponent<Transform>(camera).SetGlobalPosition(0.0f, 0.0f, -15.0f);

	// Add point lights to scene.
	int lightParent = m_scene->CreateEntity();
	m_scene->AddComponent<Transform>(lightParent);

	float halfRoot3 = 0.866025388;

	int pointLight1 = m_scene->CreateEntity();
	Transform& pointLight1Transform = m_scene->GetComponent<Transform>(pointLight1);
	pointLight1Transform.SetGlobalPosition(0.0f, 0.0f, 1.0f);
	Light& pointLight1Light = m_scene->GetComponent<Light>(pointLight1);
	pointLight1Light.SetType(POINT_LIGHT_TYPE);
	pointLight1Light.SetColor(DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));

	int pointLight2 = m_scene->CreateEntity();
	Transform& pointLight2Transform = m_scene->GetComponent<Transform>(pointLight2);
	pointLight2Transform.SetGlobalPosition(-halfRoot3, 0.0f, -0.5f);
	Light& pointLight2Light = m_scene->GetComponent<Light>(pointLight2);
	pointLight2Light.SetType(POINT_LIGHT_TYPE);
	pointLight2Light.SetColor(DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));

	int pointLight3 = m_scene->CreateEntity();
	Transform& pointLight3Transform = m_scene->GetComponent<Transform>(pointLight3);
	pointLight3Transform.SetGlobalPosition(halfRoot3, 0.0f, -0.5f);
	Light& pointLight3Light = m_scene->GetComponent<Light>(pointLight3);
	pointLight3Light.SetType(POINT_LIGHT_TYPE);
	pointLight3Light.SetColor(DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f));

	Transform& lightParentTransform = m_scene->GetComponent<Transform>(lightParent);
	lightParentTransform.AddChild(pointLight1, m_scene);
	lightParentTransform.AddChild(pointLight2, m_scene);
	lightParentTransform.AddChild(pointLight3, m_scene);
	lightParentTransform.SetGlobalScale(3.0f, 1.0f, 3.0f);
	lightParentTransform.SetGlobalPosition(0.0f, 4.0f, 0.0f);

	// Add sphere to scene.
	int sphere1 = m_scene->CreateEntity();
	Transform& sphereTransform = m_scene->GetComponent<Transform>(sphere1);
	sphereTransform.SetGlobalScale(3.0f, 3.0f, 3.0f);
	sphereTransform.SetGlobalPosition(0.0f, -2.0f, 0.0f);
	m_scene->AddComponent<Model>(sphere1);
	Model& model1 = m_scene->GetComponent<Model>(sphere1);
	model1.SetMesh(m_meshManager->GetMesh("sphere"));
	model1.SetBaseShader(m_shaderManager->GetShader<DefaultBaseShader>());
	model1.SetLightShader(m_shaderManager->GetShader<DefaultLightShader>());
	model1.SetTexture(m_textureManager->GetTexture("stoneWall"));
	model1.SetShininess(4);

	// Create room.
	int roomParent = m_scene->CreateEntity();
	m_scene->AddComponent<Transform>(roomParent);

	// Top, Bottom, Left, Right, Front, Back
	float x[] = { 1.0f, 0.0f, 0.0f };
	float y[] = { 0.0f, -1.0f, 0.0f };
	float z[] = { 0.0f, 0.0f, 1.0f };

	float xr[] = { 0.0f, 1.0f, 0.0f};
	float yr[] = { 1.0f, 0.0f, 1.0f };

	for (int i = 0; i < 6; i++) {
		int quad = m_scene->CreateEntity();
		Model& quadModel = m_scene->GetComponent<Model>(quad);
		quadModel.SetMesh(m_meshManager->GetMesh("quad"));
		quadModel.SetBaseShader(m_shaderManager->GetShader<DefaultBaseShader>());
		quadModel.SetLightShader(m_shaderManager->GetShader<DefaultLightShader>());
		quadModel.SetTexture(m_textureManager->GetTexture("stoneWall"));
		quadModel.SetShininess(4);

		float sign = i % 2 == 0 ? 1.0f : -1.0f;
		Transform& quadTransform = m_scene->GetComponent<Transform>(quad);
		quadTransform.SetGlobalPosition(sign * 0.5f * x[i / 2], sign * 0.5f * y[i / 2], sign * 0.5f * z[i / 2]);
		quadTransform.SetGlobalRotation(float(i != 4) * 90.0f * sign, xr[i / 2], yr[i / 2], 0.0f);

		m_scene->GetComponent<Transform>(roomParent).AddChild(quad, m_scene);
	}
	
	float roomSize = 10.0f;
	m_scene->GetComponent<Transform>(roomParent).SetGlobalScale(roomSize, roomSize, roomSize);

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
			DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), DirectX::XMConvertToRadians(180.0f) * dt)
		)
	);
	
	return success;
}

void Application::Resize(int width, int height, HWND hwnd)
{
	if (m_direct3d != 0) {
		m_direct3d->Resize(width, height);
	}
	m_scene->GetComponent<Camera>(0).SetAspectRatio((float)width / (float)height);
}