#pragma once

/*======
INCLUDES
======*/
#include <directxmath.h>

#include "components.h"

/*==================
FORWARD DECLARATIONS
==================*/
class Scene;

class Transform
{
public:
	Transform(int entityId);
	Transform(const Transform&);
	~Transform();
	
	int GetEntityId();

	void Update(Scene* scene);

	bool AddChild(int childId, Scene* scene);
	bool RemoveChild(int childId, Scene* scene);

	int GetParentId();
	int GetFirstChildId();
	int GetNextSiblingId();

	void SetPosition(float x, float y, float z);
	void SetScale(float x, float y, float z);
	void SetRotation(DirectX::XMVECTOR quaternion);

	DirectX::XMMATRIX GetParentMatrix();

	DirectX::XMFLOAT3 GetPosition();
	DirectX::XMFLOAT3 GetScale();
	DirectX::XMVECTOR GetRotation();

	DirectX::XMMATRIX GetModelMatrix();

public:
	static const ComponentId ID;

private:
	int m_entityId;

	bool m_dirtyFlag;

	// Entity IDs that create the hierarchy.
	int m_parent;
	int m_firstChild;
	int m_nextSibling;

	DirectX::XMMATRIX m_parentMatrix;

	DirectX::XMMATRIX m_positionMatrix;
	DirectX::XMMATRIX m_scaleMatrix;
	DirectX::XMMATRIX m_rotationMatrix;

	DirectX::XMMATRIX m_modelMatrix;
};