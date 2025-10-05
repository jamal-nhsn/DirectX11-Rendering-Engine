#include "entity.h"

Entity::Entity(int id)
	: m_id(id)
{
	m_componentMask = 0;
};

Entity::Entity(const Entity& other)
{
	m_id = other.m_id;
	m_componentMask = other.m_componentMask;
}

Entity::~Entity()
{
}

int Entity::GetId()
{
	return m_id;
}

int Entity::GetComponentMask()
{
	return m_componentMask;
}

void Entity::AddComponent(int componentId)
{
	if (!(m_componentMask & componentId)) {
		m_componentMask |= componentId;
	}
}

void Entity::RemoveComponent(int componentId)
{
	if (m_componentMask & componentId) {
		m_componentMask &= ~componentId;
	}
}

