#include "camerasystem.h"

CameraSystem::CameraSystem()
{
};

CameraSystem::CameraSystem(const CameraSystem& other)
{
}

CameraSystem::~CameraSystem()
{
}

void CameraSystem::Update(Scene* scene)
{
	std::vector<Camera3D>* camera3Ds = scene->GetComponents<Camera3D>();

	for (Camera3D& camera3D : (*camera3Ds)) {
		Transform& transform = scene->GetComponent<Transform>(camera3D.GetEntityId());
		camera3D.Update(transform.GetModelMatrix());
	}
}