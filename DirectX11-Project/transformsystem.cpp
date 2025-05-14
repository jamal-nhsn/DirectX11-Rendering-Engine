#include "transformsystem.h"

TransformSystem::TransformSystem()
{
};

TransformSystem::TransformSystem(const TransformSystem& other)
{
}

TransformSystem::~TransformSystem()
{
}

void TransformSystem::Update(Scene* scene)
{
	std::vector<Transform>* transforms = scene->GetComponents<Transform>();

	for (Transform& transform : (*transforms)) {
		// Only update roots
		if (transform.GetParentId() == -1) {
			transform.Update(scene);
		}
	}
}