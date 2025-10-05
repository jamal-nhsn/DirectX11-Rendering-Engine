#include "renderer.h"

const ComponentId Renderer::ID = ComponentId::Renderer;

Renderer::Renderer(int entityId)
	: m_entityId(entityId)
{
	m_layer = 1 << static_cast<int>(RenderLayer::Default);
}

Renderer::Renderer(const Renderer& other)
{
	m_entityId = other.m_entityId;
	m_layer = other.m_layer;
}

Renderer::~Renderer()
{
}

int Renderer::GetEntityId()
{
	return m_entityId;
}

int Renderer::GetLayer()
{
	return m_layer;
}

void Renderer::SetLayer(RenderLayer layer)
{
	m_layer = 1 << static_cast<int>(layer);
}