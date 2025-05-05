#include "transform.h"

Transform::Transform()
{
	m_dirtyFlag = false;

	m_positionMatrix = DirectX::XMMatrixIdentity();
	m_scaleMatrix    = DirectX::XMMatrixIdentity();
	m_rotationMatrix = DirectX::XMMatrixIdentity();

	m_modelMatrix = DirectX::XMMatrixIdentity();
}

Transform::Transform(const Transform&)
{
}

Transform::~Transform()
{
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
	DirectX::XMVectorSetW(m_positionMatrix.r[0], x);
	DirectX::XMVectorSetW(m_positionMatrix.r[1], y);
	DirectX::XMVectorSetW(m_positionMatrix.r[2], z);
	
	m_dirtyFlag = true;
}

void Transform::SetScale(float x, float y, float z)
{
	DirectX::XMVectorSetX(m_scaleMatrix.r[0], x);
	DirectX::XMVectorSetY(m_scaleMatrix.r[1], y);
	DirectX::XMVectorSetZ(m_scaleMatrix.r[2], z);

	m_dirtyFlag = true;
}

void Transform::SetRotation(DirectX::XMVECTOR quaternion)
{
	float x = DirectX::XMVectorGetX(quaternion);
	float y = DirectX::XMVectorGetY(quaternion);
	float z = DirectX::XMVectorGetZ(quaternion);
	float w = DirectX::XMVectorGetW(quaternion);

	DirectX::XMVectorSetX(m_rotationMatrix.r[0], 2*(x*x + y*y) - 1);
	DirectX::XMVectorSetY(m_rotationMatrix.r[0], 2*(y*z - x*w));
	DirectX::XMVectorSetZ(m_rotationMatrix.r[0], 2*(y*w + x*z));

	DirectX::XMVectorSetX(m_rotationMatrix.r[1], 2*(y*z + x*w));
	DirectX::XMVectorSetY(m_rotationMatrix.r[1], 2*(x*x + z*z) - 1);
	DirectX::XMVectorSetZ(m_rotationMatrix.r[1], 2*(z*w - x*y));

	DirectX::XMVectorSetX(m_rotationMatrix.r[2], 2*(y*w - x*z));
	DirectX::XMVectorSetY(m_rotationMatrix.r[2], 2*(z*w + x*y));
	DirectX::XMVectorSetZ(m_rotationMatrix.r[2], 2*(x*x + w*w) - 1);

	m_dirtyFlag = true;
}

DirectX::XMFLOAT3 Transform::GetPosition()
{
	return DirectX::XMFLOAT3(
		DirectX::XMVectorGetW(m_positionMatrix.r[0]),
		DirectX::XMVectorGetW(m_positionMatrix.r[1]),
		DirectX::XMVectorGetW(m_positionMatrix.r[2])
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
