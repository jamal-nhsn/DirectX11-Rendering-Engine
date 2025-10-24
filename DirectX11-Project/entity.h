#pragma once

class Entity
{
public:
	Entity(int id);
	Entity(const Entity&);
	~Entity();

	int GetId();

private:
	int m_id;
};