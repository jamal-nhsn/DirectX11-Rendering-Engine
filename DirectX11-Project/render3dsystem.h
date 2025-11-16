#pragma once

/*======
INCLUDES
======*/
#include <vector>

#include "scene.h"
#include "direct3d.h"

class Render3DSystem
{
public:
	Render3DSystem();
	Render3DSystem(const Render3DSystem&);
	~Render3DSystem();

	void Update(ID3D11Device* device, ID3D11DeviceContext* deviceContext, Scene* scene);
};