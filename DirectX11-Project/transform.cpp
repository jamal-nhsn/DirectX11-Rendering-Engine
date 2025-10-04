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

	m_localTranslation = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_localScale = DirectX::XMFLOAT3A(1.0f, 1.0f, 1.0f);
	m_localRotation = DirectX::XMQuaternionIdentity();

	m_parentTranslation = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_parentScale = DirectX::XMFLOAT3A(1.0f, 1.0f, 1.0f);
	m_parentRotation = DirectX::XMQuaternionIdentity();

	m_modelMatrix = DirectX::XMMatrixIdentity();
}

Transform::Transform(const Transform& other)
{
	m_entityId = other.m_entityId;

	m_dirtyFlag = other.m_dirtyFlag;

	m_parent = other.m_parent;
	m_firstChild = other.m_firstChild;
	m_nextSibling = other.m_nextSibling;

	m_localTranslation = other.m_localTranslation;
	m_localScale = other.m_localScale;
	m_localRotation = other.m_localRotation;

	m_parentTranslation = other.m_parentTranslation;
	m_parentScale = other.m_parentScale;
	m_parentRotation = other.m_parentRotation;

	m_modelMatrix =  other.m_modelMatrix;
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
	if (!m_dirtyFlag) {
		return;
	}

	m_modelMatrix =
		DirectX::XMMatrixScalingFromVector(DirectX::XMLoadFloat3(&m_localScale)) *
		DirectX::XMMatrixRotationQuaternion(m_localRotation) *
		DirectX::XMMatrixTranslationFromVector(DirectX::XMLoadFloat3(&m_localTranslation));

	if (m_parent != -1) {
		// Ensure parent is up to date but avoid unecessary recursion.

		if (m_entityId == 3) {
			int x = 0;
		}

		Transform& parent = scene->GetComponent<Transform>(m_parent);
		if (parent.m_dirtyFlag) {
			parent.Update(scene);
		}
		
		m_parentScale = parent.GetGlobalScale();
		m_parentRotation = parent.GetGlobalRotation();
		m_parentTranslation = parent.GetGlobalPosition();

		m_modelMatrix *= parent.m_modelMatrix;
	}

	// Mark children as dirty
	int currentChildId = m_firstChild;
	while (currentChildId != -1) {
		Transform& childTransform = scene->GetComponent<Transform>(currentChildId);
		childTransform.m_dirtyFlag = true;
		currentChildId = childTransform.m_nextSibling;
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

	// Ensure parent and child are up to date.
	Update(scene);
	child.Update(scene);

	// Remove child from existing parent if necessary.
	if (child.m_parent != -1) {
		scene->GetComponent<Transform>(child.m_parent).RemoveChild(childId, scene);
	}

	// Add child.
	child.m_parent = m_entityId;
	child.m_nextSibling = m_firstChild;
	m_firstChild = childId;

	// Store child's global data.
	DirectX::XMFLOAT3 childPosition = child.GetGlobalPosition();
	DirectX::XMFLOAT3 childScale = child.GetGlobalScale();
	DirectX::XMVECTOR childRotation = child.GetGlobalRotation();

	// Update child's parent fields.
	child.m_parentTranslation = DirectX::XMFLOAT3(
		m_localTranslation.x + m_parentTranslation.x,
		m_localTranslation.y + m_parentTranslation.y,
		m_localTranslation.z + m_parentTranslation.z
	);
	child.m_parentScale = DirectX::XMFLOAT3(
		m_localScale.x * m_parentScale.x,
		m_localScale.y * m_parentScale.y,
		m_localScale.z * m_parentScale.z
	);
	child.m_parentRotation = DirectX::XMQuaternionMultiply(
		m_localRotation,
		m_parentRotation
	);

	// Retain child's global properties.
	child.SetGlobalPosition(childPosition.x, childPosition.y, childPosition.z);
	child.SetGlobalScale(childScale.x, childScale.y, childScale.z);
	child.SetGlobalRotation(childRotation);

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

	// Ensure parent and child are up to date.
	Update(scene);
	child.Update(scene);

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

	// Store child's global data.
	DirectX::XMFLOAT3 childPosition = child.GetGlobalPosition();
	DirectX::XMFLOAT3 childScale = child.GetGlobalScale();
	DirectX::XMVECTOR childRotation = child.GetGlobalRotation();

	// Update child's parent fields.
	child.m_parentTranslation = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	child.m_parentScale = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
	child.m_parentRotation = DirectX::XMQuaternionIdentity();

	// Update child's local fields.
	child.SetLocalPosition(childPosition.x, childPosition.y, childPosition.z);
	child.SetLocalScale(childScale.x, childScale.y, childScale.z);
	child.SetLocalRotation(childRotation);

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

void Transform::SetLocalPosition(float x, float y, float z)
{
	m_localTranslation.x = x;
	m_localTranslation.y = y;
	m_localTranslation.z = z;
	m_dirtyFlag = true;
}

void Transform::SetLocalScale(float x, float y, float z)
{
	m_localScale.x = x;
	m_localScale.y = y;
	m_localScale.z = z;
	m_dirtyFlag = true;
}

void Transform::SetLocalRotation(DirectX::XMVECTOR quaternion)
{
	m_localRotation = quaternion;
	m_dirtyFlag = true;
}

void Transform::SetLocalRotation(float deg, float x, float y, float z)
{
	float rad = DirectX::XMConvertToRadians(deg);
	SetLocalRotation(DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(x, y, z, 0.0f), rad));
}

void Transform::SetGlobalPosition(float x, float y, float z)
{
	DirectX::XMVECTOR position = DirectX::XMVector3Transform(
		DirectX::XMVectorSet(x, y, z, 1.0f),
		DirectX::XMMatrixInverse(
			0,
			DirectX::XMMatrixScalingFromVector(DirectX::XMLoadFloat3(&m_parentScale)) *
			DirectX::XMMatrixRotationQuaternion(m_parentRotation) *
			DirectX::XMMatrixTranslationFromVector(DirectX::XMLoadFloat3(&m_parentTranslation))
		)
	);

	SetLocalPosition(
		DirectX::XMVectorGetX(position),
		DirectX::XMVectorGetY(position),
		DirectX::XMVectorGetZ(position)
	);
}

void Transform::SetGlobalScale(float x, float y, float z)
{
	SetLocalScale(
		x / m_parentScale.x,
		y / m_parentScale.y,
		z / m_parentScale.z
	);
}

void Transform::SetGlobalRotation(DirectX::XMVECTOR quaternion)
{
	SetLocalRotation(
		DirectX::XMQuaternionMultiply(quaternion, DirectX::XMQuaternionInverse(m_parentRotation))
	);
}

void Transform::SetGlobalRotation(float deg, float x, float y, float z)
{
	float rad = DirectX::XMConvertToRadians(deg);
	DirectX::XMVECTOR rotation = DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(x, y, z, 0.0f), rad);
	SetGlobalRotation(rotation);
}

DirectX::XMFLOAT3 Transform::GetLocalPosition()
{
	return m_localTranslation;
}

DirectX::XMFLOAT3 Transform::GetLocalScale()
{
	return m_localScale;
}

DirectX::XMVECTOR Transform::GetLocalRotation()
{
	return m_localRotation;
}

DirectX::XMFLOAT3 Transform::GetGlobalPosition()
{
	DirectX::XMFLOAT3 ans;
	DirectX::XMStoreFloat3(
		&ans,
		DirectX::XMVector3Transform(
			DirectX::XMLoadFloat3(&m_localTranslation),
			DirectX::XMMatrixTranslationFromVector(DirectX::XMLoadFloat3(&m_parentTranslation)) *
			DirectX::XMMatrixRotationQuaternion(m_parentRotation) *
			DirectX::XMMatrixScalingFromVector(DirectX::XMLoadFloat3(&m_parentScale))
		)
	);
	return ans;
}

DirectX::XMFLOAT3 Transform::GetGlobalScale()
{
	return DirectX::XMFLOAT3(
		m_localScale.x * m_parentScale.x,
		m_localScale.y * m_parentScale.y,
		m_localScale.z * m_parentScale.z
	);
}

DirectX::XMVECTOR Transform::GetGlobalRotation()
{
	return DirectX::XMQuaternionMultiply(m_localRotation, m_parentRotation);
}

DirectX::XMMATRIX Transform::GetModelMatrix()
{
	return m_modelMatrix;
}

DirectX::XMFLOAT3 Transform::GetForward()
{
	DirectX::XMFLOAT3 ans;
	DirectX::XMStoreFloat3(
		&ans,
		DirectX::XMVector3Rotate(DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), GetGlobalRotation())
	);
	return ans;
}
