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
	std::vector<Camera>* cameras = scene->GetComponents<Camera>();

	for (Camera& camera : (*cameras)) {
		Transform& transform = scene->GetComponent<Transform>(camera.GetEntityId());
		camera.Update(transform.GetModelMatrix());
	}
}