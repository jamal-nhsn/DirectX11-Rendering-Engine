#include "scene.h"

Scene::Scene()
{
	m_componentLists[static_cast<int>(ComponentId::Transform)] = &m_transforms;
	m_componentLists[static_cast<int>(ComponentId::Model)]     = &m_models;
	m_componentLists[static_cast<int>(ComponentId::Camera)]    = &m_cameras;
	m_componentLists[static_cast<int>(ComponentId::Light)]     = &m_lights;
	m_componentLists[static_cast<int>(ComponentId::Renderer)]  = &m_renderers;

	m_ambientLight = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
}

Scene::Scene(const Scene& other)
{
}

Scene::~Scene()
{
}

void Scene::Initialize(float screenWidth, float screenHeight)
{
	int camera = CreateEntity();
	AddComponent<Transform>(camera);
	GetComponent<Transform>(camera).SetLocalPosition(0.0f, 0.0f, -5.0f);

	AddComponent<Camera>(camera);
	GetComponent<Camera>(camera).SetAspectRatio(screenWidth/screenHeight);
}

int Scene::CreateEntity()
{
	m_entities.emplace_back(m_entities.size());

	// Reserve space for all components even if the entity wont use them all.
	m_components.resize(m_components.size() + static_cast<int>(ComponentId::COMPONENT_COUNT), -1);

	return m_entities.size() - 1;
}

DirectX::XMFLOAT4 Scene::GetAmbientLight()
{
	return m_ambientLight;
}

void Scene::SetAmbientLight(DirectX::XMFLOAT4 color)
{
	m_ambientLight = color;
}