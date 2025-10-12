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

void CameraSystem::Update(Direct3D* direct3d, Scene* scene)
{
	std::vector<Camera3D>* camera3Ds = scene->GetComponents<Camera3D>();
	std::vector<Camera2D>* camera2Ds = scene->GetComponents<Camera2D>();

	for (Camera3D& camera3D : (*camera3Ds)) {
		Transform& transform = scene->GetComponent<Transform>(camera3D.GetEntityId());
		camera3D.Update(transform.GetModelMatrix(), direct3d->GetViewWidth(), direct3d->GetViewHeight());
	}

	for (Camera2D& camera2D : (*camera2Ds)) {
		Transform& transform = scene->GetComponent<Transform>(camera2D.GetEntityId());
		camera2D.Update(transform.GetModelMatrix(), direct3d->GetViewWidth(), direct3d->GetViewHeight());
	}
}