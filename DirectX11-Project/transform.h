#pragma once

/*======
INCLUDES
======*/
#include <directxmath.h>

class Transform
{
public:
	Transform();
	Transform(const Transform&);
	~Transform();
	
	void Update();

	void SetPosition(float x, float y, float z);
	void SetScale(float x, float y, float z);
	void SetRotation(DirectX::XMVECTOR quaternion);

	DirectX::XMFLOAT3 GetPosition();
	DirectX::XMFLOAT3 GetScale();
	DirectX::XMVECTOR GetRotation();

	DirectX::XMMATRIX GetModelMatrix();

private:
	bool m_dirtyFlag;

	DirectX::XMMATRIX m_positionMatrix;
	DirectX::XMMATRIX m_scaleMatrix;
	DirectX::XMMATRIX m_rotationMatrix;

	DirectX::XMMATRIX m_modelMatrix;
};