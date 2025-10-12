#pragma once

/*======
INCLUDES
======*/
#include <directxmath.h>
#include <cmath>

#include "components.h"
#include "renderer.h"

class Camera2D
{
public:
	Camera2D(int entityId);
	Camera2D(const Camera2D&);
	~Camera2D();

	int GetEntityId();

	void Update(DirectX::XMMATRIX modelMatrix, float viewWidth, float viewHeight);

	int GetRenderMask();
	float GetViewWidth();
	float GetViewHeight();
	float GetNearPlane();
	float GetFarPlane();

	void SetRenderMask(int renderMask);
	void SetViewWidth(float viewWidth);
	void SetViewHeight(float viewHeight);
	void SetNearPlane(float nearPlane);
	void SetFarPlane(float farPlane);

	DirectX::XMMATRIX GetViewMatrix();
	DirectX::XMMATRIX GetProjectionMatrix();
	DirectX::XMMATRIX GetMatrix();

public:
	static const ComponentId ID;

private:
	int m_entityId;

	int m_renderMask;

	float m_viewWidth;
	float m_viewHeight;
	float m_nearPlane;
	float m_farPlane;

	DirectX::XMMATRIX m_viewMatrix;
	DirectX::XMMATRIX m_projectionMatrix;
	DirectX::XMMATRIX m_matrix;
};