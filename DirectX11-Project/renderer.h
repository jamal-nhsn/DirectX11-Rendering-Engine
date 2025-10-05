#pragma once

/*======
INCLUDES
======*/
#include "components.h"

enum class RenderLayer
{
	Default,
	UI
};

class Renderer
{
public:
	Renderer(int entityId);
	Renderer(const Renderer& other);
	~Renderer();

	int GetEntityId();

	void SetLayer(RenderLayer layer);
	int GetLayer();

public:
	static const ComponentId ID;

private:
	int m_entityId;

	int m_layer;
};