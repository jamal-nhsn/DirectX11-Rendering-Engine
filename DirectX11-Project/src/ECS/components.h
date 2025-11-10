#pragma once

#include "../../mesh.h"
#include <memory>

struct TransformComponent
{
	
};

struct ModelComponent
{
	std::shared_ptr<Mesh> mesh;
};

struct MaterialProperties
{

};