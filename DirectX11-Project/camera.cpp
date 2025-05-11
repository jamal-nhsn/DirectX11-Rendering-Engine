#include "camera.h"

const ComponentId Camera::ID = ComponentId::Camera;

Camera::Camera(int entityId)
	: m_entityId(entityId)
{
	m_dirtyFlag = true;

	// Default camera settings:
	m_fovy = DirectX::XMConvertToRadians(60.0f);
	m_aspectRatio = 1.0f;
	m_nearPlane = 0.1f;
	m_farPlane = 1000.0f;

	m_viewMatrix = DirectX::XMMatrixIdentity();
	m_projectionMatrix = DirectX::XMMatrixIdentity();
	m_matrix = DirectX::XMMatrixIdentity();
}

Camera::Camera(const Camera& other)
{
	m_entityId = other.m_entityId;
	m_dirtyFlag = other.m_dirtyFlag;
	m_fovy = other.m_fovy;
	m_aspectRatio = other.m_aspectRatio;
	m_nearPlane = other.m_nearPlane;
	m_farPlane = other.m_farPlane;
	m_viewMatrix = other.m_viewMatrix;
	m_projectionMatrix = other.m_projectionMatrix;
	m_matrix = other.m_matrix;
}

Camera::~Camera()
{
}

int Camera::GetEntityId()
{
	return m_entityId;
}

void Camera::Update(DirectX::XMMATRIX modelMatrix)
{
	m_viewMatrix = DirectX::XMMatrixInverse(NULL, modelMatrix);
	m_matrix = DirectX::XMMatrixMultiply(m_viewMatrix, m_projectionMatrix);

	if (!m_dirtyFlag) {
		return;
	}

	m_projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(m_fovy, m_aspectRatio, m_nearPlane, m_farPlane);
	
	m_dirtyFlag = false;
}

float Camera::GetFovY()
{
	return m_fovy;
}

float Camera::GetAspectRatio()
{
	return m_aspectRatio;
}

float Camera::GetNearPlane()
{
	return m_nearPlane;
}

float Camera::GetFarPlane()
{
	return m_farPlane;
}

void Camera::SetFovY(float fovy)
{
	float newFovY = DirectX::XMConvertToRadians(fovy);
	m_dirtyFlag = m_fovy != newFovY;
	m_fovy = newFovY;
}

void Camera::SetAspectRatio(float aspectRatio)
{
	m_dirtyFlag = m_aspectRatio != aspectRatio;
	m_aspectRatio = aspectRatio;
}

void Camera::SetNearPlane(float nearPlane)
{
	m_dirtyFlag = m_nearPlane != nearPlane;
	m_nearPlane = nearPlane;
}

void Camera::SetFarPlane(float farPlane)
{
	m_dirtyFlag = m_farPlane != farPlane;
	m_farPlane = farPlane;
}

DirectX::XMMATRIX Camera::GetViewMatrix()
{
	return m_viewMatrix;
}

DirectX::XMMATRIX Camera::GetProjectionMatrix()
{
	return m_projectionMatrix;
}

DirectX::XMMATRIX Camera::GetMatrix()
{
	return m_matrix;
}