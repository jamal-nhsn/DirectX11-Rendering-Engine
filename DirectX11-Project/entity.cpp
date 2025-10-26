#include "entity.h"

Entity::Entity(int id)
	: m_id(id)
{
};

Entity::Entity(const Entity& other)
{
	m_id = other.m_id;
}

Entity::~Entity()
{
}

int Entity::GetId()
{
	return m_id;
}

