#include "transform.h"

Transform::Transform()
{
	m_dirtyFlag = false;

	m_positionMatrix = XMMatrixIdentity();
	m_scaleMatrix    = XMMatrixIdentity();
	m_rotationMatrix     = XMMatrixIdentity();

	m_modelMatrix = XMMatrixIdentity();
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
	XMVectorSetW(m_positionMatrix.r[0], x);
	XMVectorSetW(m_positionMatrix.r[1], y);
	XMVectorSetW(m_positionMatrix.r[2], z);
	
	m_dirtyFlag = true;
}

void Transform::SetScale(float x, float y, float z)
{
	XMVectorSetX(m_scaleMatrix.r[0], x);
	XMVectorSetY(m_scaleMatrix.r[1], y);
	XMVectorSetZ(m_scaleMatrix.r[2], z);

	m_dirtyFlag = true;
}

void Transform::SetRotation(XMVECTOR quaternion)
{
	float x = XMVectorGetX(quaternion);
	float y = XMVectorGetY(quaternion);
	float z = XMVectorGetZ(quaternion);
	float w = XMVectorGetW(quaternion);

	XMVectorSetX(m_rotationMatrix.r[0], 2*(x*x + y*y) - 1);
	XMVectorSetY(m_rotationMatrix.r[0], 2*(y*z - x*w));
	XMVectorSetZ(m_rotationMatrix.r[0], 2*(y*w + x*z));

	XMVectorSetX(m_rotationMatrix.r[1], 2*(y*z + x*w));
	XMVectorSetY(m_rotationMatrix.r[1], 2*(x*x + z*z) - 1);
	XMVectorSetZ(m_rotationMatrix.r[1], 2*(z*w - x*y));

	XMVectorSetX(m_rotationMatrix.r[2], 2*(y*w - x*z));
	XMVectorSetY(m_rotationMatrix.r[2], 2*(z*w + x*y));
	XMVectorSetZ(m_rotationMatrix.r[2], 2*(x*x + w*w) - 1);

	m_dirtyFlag = true;
}

XMFLOAT3 Transform::GetPosition()
{
	return XMFLOAT3(
		XMVectorGetW(m_positionMatrix.r[0]),
		XMVectorGetW(m_positionMatrix.r[1]),
		XMVectorGetW(m_positionMatrix.r[2])
	);
}

XMFLOAT3 Transform::GetScale()
{
	return XMFLOAT3(
		XMVectorGetX(m_scaleMatrix.r[0]),
		XMVectorGetY(m_scaleMatrix.r[1]),
		XMVectorGetZ(m_scaleMatrix.r[2])
	);
}

XMVECTOR Transform::GetRotation()
{
	return XMQuaternionRotationMatrix(m_rotationMatrix);
}

XMMATRIX Transform::GetModelMatrix()
{
	return m_modelMatrix;
}
