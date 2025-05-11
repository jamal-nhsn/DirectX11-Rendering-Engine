#pragma once

class Entity
{
public:
	Entity(int id);
	Entity(const Entity&);
	~Entity();

	int GetId();
	int GetComponentMask();

	void AddComponent(int componentId);
	void RemoveComponent(int componentId);

private:
	int m_id;
	int m_componentMask;
};