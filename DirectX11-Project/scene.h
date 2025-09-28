#pragma once

/*======
INCLUDES
======*/
#include <vector>

#include "entity.h"
#include "transform.h"
#include "model.h"
#include "camera.h"
#include "light.h"

class Scene
{
public:
	Scene();
	Scene(const Scene&);
	~Scene();

	void Initialize(float screenWidth, float screenHeight);

	int CreateEntity();

	template<typename ComponentType>
	bool AddComponent(int entityId);

	template<typename ComponentType>
	bool RemoveComponent(int entityId);

	template<typename ComponentType>
	ComponentType& GetComponent(int entityId);

	template<typename ComponentType>
	std::vector<ComponentType>* GetComponents();

	template<typename ComponentType>
	bool HasComponent(int entityId);

	DirectX::XMFLOAT4 GetAmbientLight();
	void SetAmbientLight(DirectX::XMFLOAT4 color);

private:
	std::vector<Entity> m_entities;   // All entities in scene.
	std::vector<int>    m_components; // Indicies into component lists for each entity.

	DirectX::XMFLOAT4 m_ambientLight;

	// All component lists.
	std::vector<Transform> m_transforms;
	std::vector<Model>     m_models;
	std::vector<Camera>	   m_cameras;
	std::vector<Light>     m_lights;

	void* m_componentLists[static_cast<int>(ComponentId::COMPONENT_COUNT)]; // Table of component lists for generalized access.
};

template<typename ComponentType>
bool Scene::AddComponent(int entityId)
{
	bool successful = !HasComponent<ComponentType>(entityId);
	if (successful) {
		m_entities[entityId].AddComponent(1 << static_cast<int>(ComponentType::ID));
		std::vector<ComponentType>* componentList = static_cast<std::vector<ComponentType>*>(m_componentLists[static_cast<int>(ComponentType::ID)]);
		componentList->emplace_back(entityId);
		m_components[entityId * static_cast<int>(ComponentId::COMPONENT_COUNT) + static_cast<int>(ComponentType::ID)] = static_cast<int>(componentList->size()) - 1;
	}
	return successful;
}

template<typename ComponentType>
bool Scene::RemoveComponent(int entityId)
{
	bool successful = HasComponent<ComponentType>(entityId);
	if (successful) {
		int index = m_components[entityId * static_cast<int>(ComponentId::COMPONENT_COUNT) + static_cast<int>(ComponentType::ID)];
		std::vector<ComponentType>* componentList = static_cast<std::vector<ComponentType>*>(m_componentLists[static_cast<int>(ComponentType::ID)]);

		if (index < componentList->size()) {
			ComponentType toRemove = (*componentList)[index];
			ComponentType back = componentList->back();

			(*componentList)[componentList->size() - 1] = toRemove;
			(*componentList)[index] = back;

			m_components[back.GetEntityId() * static_cast<int>(ComponentId::COMPONENT_COUNT) + static_cast<int>(ComponentType::ID)] = index;
		}

		m_components[entityId * static_cast<int>(ComponentId::COMPONENT_COUNT) + static_cast<int>(ComponentType::ID)] = -1;
		componentList->pop_back();

		m_entities[entityId].RemoveComponent(1 << static_cast<int>(ComponentType::ID));
	}
	return successful;
}

template<typename ComponentType>
ComponentType& Scene::GetComponent(int entityId)
{
	int index = m_components[entityId * static_cast<int>(ComponentId::COMPONENT_COUNT) + static_cast<int>(ComponentType::ID)];
	return (*static_cast<std::vector<ComponentType>*>(m_componentLists[static_cast<int>(ComponentType::ID)]))[index];
}

template<typename ComponentType>
std::vector<ComponentType>* Scene::GetComponents()
{
	return static_cast<std::vector<ComponentType>*>(m_componentLists[static_cast<int>(ComponentType::ID)]);
}

template<typename ComponentType>
bool Scene::HasComponent(int entityId)
{
	return (m_entities[entityId].GetComponentMask() & (1 << static_cast<int>(ComponentType::ID))) != 0;
}