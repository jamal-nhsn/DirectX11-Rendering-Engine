#include "camera3d.h"

const ComponentId Camera3D::ID = ComponentId::Camera3D;

Camera3D::Camera3D(int entityId)
	: m_entityId(entityId)
{
	// Default Camera3D settings:
	m_renderMask = 1 << static_cast<int>(RenderLayer::Default);

	m_fovy = DirectX::XMConvertToRadians(60.0f);
	m_aspectRatio = 1.0f;
	m_nearPlane = 0.1f;
	m_farPlane = 1000.0f;

	m_viewMatrix = DirectX::XMMatrixIdentity();
	m_projectionMatrix = DirectX::XMMatrixIdentity();
	m_matrix = DirectX::XMMatrixIdentity();
}

Camera3D::Camera3D(const Camera3D& other)
{
	m_entityId = other.m_entityId;
	m_renderMask = other.m_renderMask;
	m_fovy = other.m_fovy;
	m_aspectRatio = other.m_aspectRatio;
	m_nearPlane = other.m_nearPlane;
	m_farPlane = other.m_farPlane;
	m_viewMatrix = other.m_viewMatrix;
	m_projectionMatrix = other.m_projectionMatrix;
	m_matrix = other.m_matrix;
}

Camera3D::~Camera3D()
{
}

int Camera3D::GetEntityId()
{
	return m_entityId;
}

void Camera3D::Update(DirectX::XMMATRIX modelMatrix, float viewWidth, float viewHeight)
{
	m_aspectRatio = viewWidth / viewHeight;

	m_viewMatrix = DirectX::XMMatrixInverse(NULL, modelMatrix);
	m_projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(m_fovy, m_aspectRatio, m_nearPlane, m_farPlane);

	m_matrix = DirectX::XMMatrixMultiply(m_viewMatrix, m_projectionMatrix);
}

int Camera3D::GetRenderMask()
{
	return m_renderMask;
}

float Camera3D::GetFovY()
{
	return m_fovy;
}

float Camera3D::GetAspectRatio()
{
	return m_aspectRatio;
}

float Camera3D::GetNearPlane()
{
	return m_nearPlane;
}

float Camera3D::GetFarPlane()
{
	return m_farPlane;
}

void Camera3D::SetRenderMask(int renderMask)
{
	m_renderMask = renderMask;
}

void Camera3D::SetFovY(float fovy)
{
	float newFovY = DirectX::XMConvertToRadians(fovy);
	m_fovy = newFovY;
}

void Camera3D::SetAspectRatio(float aspectRatio)
{
	m_aspectRatio = aspectRatio;
}

void Camera3D::SetNearPlane(float nearPlane)
{
	m_nearPlane = nearPlane;
}

void Camera3D::SetFarPlane(float farPlane)
{
	m_farPlane = farPlane;
}

DirectX::XMMATRIX Camera3D::GetViewMatrix()
{
	return m_viewMatrix;
}

DirectX::XMMATRIX Camera3D::GetProjectionMatrix()
{
	return m_projectionMatrix;
}

DirectX::XMMATRIX Camera3D::GetMatrix()
{
	return m_matrix;
}