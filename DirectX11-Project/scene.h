#pragma once

/*======
INCLUDES
======*/
#include <vector>

#include "entity.h"
#include "transform.h"
#include "model.h"
#include "camera3d.h"
#include "camera2d.h"
#include "light.h"
#include "renderer.h"
#include "sprite.h"
#include "spriteanimator.h"
#include "text.h"

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
	std::vector<Transform>      m_transforms;
	std::vector<Model>          m_models;
	std::vector<Camera3D>       m_camera3Ds;
	std::vector<Camera2D>       m_camera2Ds;
	std::vector<Light>          m_lights;
	std::vector<Renderer>       m_renderers;
	std::vector<Sprite>         m_sprites;
	std::vector<SpriteAnimator> m_spriteAnimators;
	std::vector<Text>           m_texts;

	void* m_componentLists[static_cast<int>(ComponentId::COMPONENT_COUNT)]; // Table of component lists for generalized access.
};

template<typename ComponentType>
bool Scene::AddComponent(int entityId)
{
	bool successful = !HasComponent<ComponentType>(entityId);
	if (successful) {
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
	}
	return successful;
}

template<typename ComponentType>
ComponentType& Scene::GetComponent(int entityId)
{
	if (!HasComponent<ComponentType>(entityId)) {
		AddComponent<ComponentType>(entityId);
	}
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
	int index = m_components[entityId * static_cast<int>(ComponentId::COMPONENT_COUNT) + static_cast<int>(ComponentType::ID)];
	return index != -1;
}