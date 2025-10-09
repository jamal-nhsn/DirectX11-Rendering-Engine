#include "camera2d.h"

const ComponentId Camera2D::ID = ComponentId::Camera2D;

Camera2D::Camera2D(int entityId)
	: m_entityId(entityId)
{
	m_dirtyFlag = true;

	// Default Camera2D settings:
	m_renderMask = 1 << static_cast<int>(RenderLayer::Default);

	m_fovy = DirectX::XMConvertToRadians(60.0f);
	m_aspectRatio = 1.0f;
	m_nearPlane = 0.1f;
	m_farPlane = 1000.0f;

	m_viewMatrix = DirectX::XMMatrixIdentity();
	m_projectionMatrix = DirectX::XMMatrixIdentity();
	m_matrix = DirectX::XMMatrixIdentity();
}

Camera2D::Camera2D(const Camera2D& other)
{
	m_entityId = other.m_entityId;
	m_dirtyFlag = other.m_dirtyFlag;
	m_renderMask = other.m_renderMask;
	m_fovy = other.m_fovy;
	m_aspectRatio = other.m_aspectRatio;
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

void Camera2D::Update(DirectX::XMMATRIX modelMatrix, int screenWidth, int screenHeight)
{
	m_viewMatrix = DirectX::XMMatrixInverse(NULL, modelMatrix);
	m_matrix = DirectX::XMMatrixMultiply(m_viewMatrix, m_projectionMatrix);

	if (!m_dirtyFlag) {
		return;
	}

	m_projectionMatrix = DirectX::XMMatrixOrthographicLH(static_cast<float>(screenWidth), static_cast<float>(screenHeight), m_nearPlane, m_farPlane);

	m_dirtyFlag = false;
}

int Camera2D::GetRenderMask()
{
	return m_renderMask;
}

float Camera2D::GetFovY()
{
	return m_fovy;
}

float Camera2D::GetAspectRatio()
{
	return m_aspectRatio;
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

void Camera2D::SetFovY(float fovy)
{
	float newFovY = DirectX::XMConvertToRadians(fovy);
	m_dirtyFlag = m_fovy != newFovY;
	m_fovy = newFovY;
}

void Camera2D::SetAspectRatio(float aspectRatio)
{
	m_dirtyFlag = m_aspectRatio != aspectRatio;
	m_aspectRatio = aspectRatio;
}

void Camera2D::SetNearPlane(float nearPlane)
{
	m_dirtyFlag = m_nearPlane != nearPlane;
	m_nearPlane = nearPlane;
}

void Camera2D::SetFarPlane(float farPlane)
{
	m_dirtyFlag = m_farPlane != farPlane;
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