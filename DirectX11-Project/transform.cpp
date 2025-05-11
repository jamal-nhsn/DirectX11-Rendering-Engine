#include "transform.h"

const ComponentId Transform::ID = ComponentId::Transform;

Transform::Transform(int entityId)
	: m_entityId(entityId)
{
	m_dirtyFlag = false;

	m_positionMatrix = DirectX::XMMatrixIdentity();
	m_scaleMatrix    = DirectX::XMMatrixIdentity();
	m_rotationMatrix = DirectX::XMMatrixIdentity();

	m_modelMatrix = DirectX::XMMatrixIdentity();
}

Transform::Transform(const Transform& other)
{
	m_entityId = other.m_entityId;
	m_dirtyFlag = other.m_dirtyFlag;
	m_positionMatrix = other.m_positionMatrix;
	m_scaleMatrix = other.m_scaleMatrix;
	m_rotationMatrix = other.m_rotationMatrix;
	m_modelMatrix = other.m_modelMatrix;
}

Transform::~Transform()
{
}

int Transform::GetEntityId()
{
	return m_entityId;
}

void Transform::Update()
{
	if (m_dirtyFlag) {
		m_modelMatrix = XMMatrixMultiply(m_positionMatrix, XMMatrixMultiply(m_scaleMatrix, m_rotationMatrix));
		m_dirtyFlag = false;
	}
}

void Transform::SetPosition(float x, float y, float z)
{
	m_positionMatrix = DirectX::XMMatrixTranslation(x, y, z);
	m_dirtyFlag = true;
}

void Transform::SetScale(float x, float y, float z)
{
	m_scaleMatrix = DirectX::XMMatrixScaling(x, y, z);
	m_dirtyFlag = true;
}

void Transform::SetRotation(DirectX::XMVECTOR quaternion)
{
	m_rotationMatrix = DirectX::XMMatrixRotationQuaternion(quaternion);
	m_dirtyFlag = true;
}

DirectX::XMFLOAT3 Transform::GetPosition()
{
	return DirectX::XMFLOAT3(
		DirectX::XMVectorGetX(m_positionMatrix.r[3]),
		DirectX::XMVectorGetY(m_positionMatrix.r[3]),
		DirectX::XMVectorGetZ(m_positionMatrix.r[3])
	);
}

DirectX::XMFLOAT3 Transform::GetScale()
{
	return DirectX::XMFLOAT3(
		DirectX::XMVectorGetX(m_scaleMatrix.r[0]),
		DirectX::XMVectorGetY(m_scaleMatrix.r[1]),
		DirectX::XMVectorGetZ(m_scaleMatrix.r[2])
	);
}

DirectX::XMVECTOR Transform::GetRotation()
{
	return XMQuaternionRotationMatrix(m_rotationMatrix);
}

DirectX::XMMATRIX Transform::GetModelMatrix()
{
	return m_modelMatrix;
}
