#include "camera2d.h"

const ComponentId Camera2D::ID = ComponentId::Camera2D;

Camera2D::Camera2D(int entityId)
	: m_entityId(entityId)
{
	// Default Camera2D settings:
	m_renderMask = 1 << static_cast<int>(RenderLayer::Default);

	m_viewWidth = 1.0f;
	m_viewHeight = 1.0f;
	m_nearPlane = 0.1f;
	m_farPlane = 1000.0f;

	m_viewMatrix = DirectX::XMMatrixIdentity();
	m_projectionMatrix = DirectX::XMMatrixIdentity();
	m_matrix = DirectX::XMMatrixIdentity();
}

Camera2D::Camera2D(const Camera2D& other)
{
	m_entityId = other.m_entityId;
	m_renderMask = other.m_renderMask;
	m_viewWidth = other.m_viewWidth;
	m_viewHeight = other.m_viewHeight;
	m_nearPlane = other.m_nearPlane;
	m_farPlane = other.m_farPlane;
	m_viewMatrix = other.m_viewMatrix;
	m_projectionMatrix = other.m_projectionMatrix;
	m_matrix = other.m_matrix;
}

Camera2D::~Camera2D()
{
}

int Camera2D::GetEntityId()
{
	return m_entityId;
}

void Camera2D::Update(DirectX::XMMATRIX modelMatrix, float viewWidth, float viewHeight)
{
	m_viewWidth = viewWidth;
	m_viewHeight = viewHeight;

	m_viewMatrix = DirectX::XMMatrixInverse(NULL, modelMatrix);
	//m_projectionMatrix = DirectX::XMMatrixOrthographicLH(m_viewWidth, m_viewHeight, m_nearPlane, m_farPlane);
	m_projectionMatrix = DirectX::XMMatrixOrthographicOffCenterLH(0.0f, m_viewWidth, 0.0f, m_viewHeight, m_nearPlane, m_farPlane);

	m_matrix = DirectX::XMMatrixMultiply(m_viewMatrix, m_projectionMatrix);
}

int Camera2D::GetRenderMask()
{
	return m_renderMask;
}

float Camera2D::GetViewWidth()
{
	return m_viewWidth;
}

float Camera2D::GetViewHeight()
{
	return m_viewHeight;
}

float Camera2D::GetNearPlane()
{
	return m_nearPlane;
}

float Camera2D::GetFarPlane()
{
	return m_farPlane;
}

void Camera2D::SetRenderMask(int renderMask)
{
	m_renderMask = renderMask;
}

void Camera2D::SetViewWidth(float viewWidth)
{
	m_viewWidth = viewWidth;
}

void Camera2D::SetViewHeight(float viewHeight)
{
	m_viewHeight = viewHeight;
}

void Camera2D::SetNearPlane(float nearPlane)
{
	m_nearPlane = nearPlane;
}

void Camera2D::SetFarPlane(float farPlane)
{
	m_farPlane = farPlane;
}

DirectX::XMMATRIX Camera2D::GetViewMatrix()
{
	return m_viewMatrix;
}

DirectX::XMMATRIX Camera2D::GetProjectionMatrix()
{
	return m_projectionMatrix;
}

DirectX::XMMATRIX Camera2D::GetMatrix()
{
	return m_matrix;
}