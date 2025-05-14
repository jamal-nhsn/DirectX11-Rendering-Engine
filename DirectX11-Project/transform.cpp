#include "transform.h"
#include "scene.h"


const ComponentId Transform::ID = ComponentId::Transform;

Transform::Transform(int entityId)
	: m_entityId(entityId)
{
	m_dirtyFlag = false;

	m_parent = -1;
	m_firstChild = -1;
	m_nextSibling = -1;

	m_parentMatrix   = DirectX::XMMatrixIdentity();
	m_positionMatrix = DirectX::XMMatrixIdentity();
	m_scaleMatrix    = DirectX::XMMatrixIdentity();
	m_rotationMatrix = DirectX::XMMatrixIdentity();

	m_modelMatrix = DirectX::XMMatrixIdentity();
}

Transform::Transform(const Transform& other)
{
	m_entityId = other.m_entityId;

	m_dirtyFlag = other.m_dirtyFlag;

	m_parent = other.m_parent;
	m_firstChild = other.m_firstChild;
	m_nextSibling = other.m_nextSibling;

	m_parentMatrix = other.m_parentMatrix;
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

void Transform::Update(Scene* scene)
{
	
	if (m_entityId == 1) {
		DirectX::XMVECTOR axis = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
		constexpr float angle = DirectX::XMConvertToRadians(0.1);
		DirectX::XMVECTOR rotation = DirectX::XMQuaternionRotationAxis(axis, angle);

		SetRotation(DirectX::XMQuaternionMultiply(GetRotation(), rotation));
	}
	
	if (!m_dirtyFlag) {
		return;
	}

	m_parentMatrix = m_parent == -1 ? DirectX::XMMatrixIdentity() : scene->GetComponent<Transform>(m_parent).GetModelMatrix();
	m_modelMatrix = (m_scaleMatrix * m_rotationMatrix * m_positionMatrix) * m_parentMatrix;

	int currentChildId = m_firstChild;
	while (currentChildId != -1) {
		Transform& currentChild = scene->GetComponent<Transform>(currentChildId);
		currentChild.m_dirtyFlag = true;
		currentChild.Update(scene);
		currentChildId = currentChild.m_nextSibling;
	}

	m_dirtyFlag = false;
}

bool Transform::AddChild(int childId, Scene* scene)
{
	// Check if the child is an ancestor of the current node or is the current node.
	int root = m_entityId;
	while (root != -1) {
		if (root == childId) {
			return false;
		}
		root = scene->GetComponent<Transform>(root).m_parent;
	}

	Transform& child = scene->GetComponent<Transform>(childId);

	// Remove child from existing parent if necessary.
	if (child.m_parent != -1) {
		scene->GetComponent<Transform>(child.m_parent).RemoveChild(childId, scene);
	}

	// Add child.
	child.m_parent = m_entityId;
	child.m_nextSibling = m_firstChild;
	m_firstChild = childId;

	return true;
}

bool Transform::RemoveChild(int childId, Scene* scene)
{
	// Find child location and previous sibling in list.
	int currNode = m_firstChild;
	int prevNode = -1;
	while (currNode != -1 && currNode != childId) {
		prevNode = currNode;
		currNode = scene->GetComponent<Transform>(currNode).m_nextSibling;
	}

	// Node is not the parent of this child.
	if (currNode == -1) {
		return false;
	}

	Transform& child = scene->GetComponent<Transform>(childId);

	// Child is the first child.
	if (prevNode == -1) {
		m_firstChild = child.m_nextSibling;
	}
	// Remove from middle of the list.
	else {
		scene->GetComponent<Transform>(prevNode).m_nextSibling = child.m_nextSibling;
	}

	// Remove child.
	child.m_parent = -1;
	child.m_nextSibling = -1;

	return true;
}

int Transform::GetParentId()
{
	return m_parent;
}

int Transform::GetFirstChildId()
{
	return m_firstChild;
}

int Transform::GetNextSiblingId() {
	return m_nextSibling;
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

DirectX::XMMATRIX Transform::GetParentMatrix()
{
	return m_parentMatrix;
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
