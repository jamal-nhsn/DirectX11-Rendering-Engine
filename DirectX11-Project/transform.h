#pragma once

/*======
INCLUDES
======*/
#include <directxmath.h>
using namespace DirectX;

class Transform
{
public:
	Transform();
	Transform(const Transform&);
	~Transform();
	
	void Update();

	void SetPosition(float x, float y, float z);
	void SetScale(float x, float y, float z);
	void SetRotation(XMVECTOR quaternion);

	XMFLOAT3 GetPosition();
	XMFLOAT3 GetScale();
	XMVECTOR GetRotation();

	XMMATRIX GetModelMatrix();

private:
	bool m_dirtyFlag;

	XMMATRIX m_positionMatrix;
	XMMATRIX m_scaleMatrix;
	XMMATRIX m_rotationMatrix;

	XMMATRIX m_modelMatrix;
};