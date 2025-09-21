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

	void SetLocalPosition(float x, float y, float z);
	void SetLocalScale(float x, float y, float z);
	void SetLocalRotation(DirectX::XMVECTOR quaternion);
	void SetLocalRotation(float deg, float x, float y, float z);

	void SetGlobalPosition(float x, float y, float z);
	void SetGlobalScale(float x, float y, float z);
	void SetGlobalRotation(DirectX::XMVECTOR quaternion);
	void SetGlobalRotation(float deg, float x, float y, float z);

	DirectX::XMFLOAT3 GetLocalPosition();
	DirectX::XMFLOAT3 GetLocalScale();
	DirectX::XMVECTOR GetLocalRotation();

	DirectX::XMFLOAT3 GetGlobalPosition();
	DirectX::XMFLOAT3 GetGlobalScale();
	DirectX::XMVECTOR GetGlobalRotation();

	DirectX::XMMATRIX GetModelMatrix();

	DirectX::XMFLOAT3 GetForward();

public:
	static const ComponentId ID;

private:
	int m_entityId;

	bool m_dirtyFlag;

	// Entity IDs that create the hierarchy.
	int m_parent;
	int m_firstChild;
	int m_nextSibling;

	DirectX::XMFLOAT3 m_localTranslation;
	DirectX::XMFLOAT3 m_localScale;
	DirectX::XMVECTOR m_localRotation;

	DirectX::XMFLOAT3 m_parentTranslation;
	DirectX::XMFLOAT3 m_parentScale;
	DirectX::XMVECTOR m_parentRotation;

	DirectX::XMMATRIX m_modelMatrix;
};