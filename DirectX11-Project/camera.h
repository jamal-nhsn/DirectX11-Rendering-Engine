#pragma once

/*======
INCLUDES
======*/
#include <directxmath.h>
#include <cmath>

#include "components.h"

class Camera
{
public:
	Camera(int entityId);
	Camera(const Camera&);
	~Camera();

	int GetEntityId();

	void Update(DirectX::XMMATRIX modelMatrix);

	float GetFovY();
	float GetAspectRatio();
	float GetNearPlane();
	float GetFarPlane();

	void SetFovY(float fovy);
	void SetAspectRatio(float aspectRatio);
	void SetNearPlane(float nearPlane);
	void SetFarPlane(float farPlane);

	DirectX::XMMATRIX GetViewMatrix();
	DirectX::XMMATRIX GetProjectionMatrix();
	DirectX::XMMATRIX GetMatrix();

public:
	static const ComponentId ID;

private:
	int m_entityId;

	bool m_dirtyFlag;

	float m_fovy;
	float m_aspectRatio;
	float m_nearPlane;
	float m_farPlane;

	DirectX::XMMATRIX m_viewMatrix;
	DirectX::XMMATRIX m_projectionMatrix;
	DirectX::XMMATRIX m_matrix;
};