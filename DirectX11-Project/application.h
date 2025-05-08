#pragma once

/*======
INCLUDES
======*/
#include "direct3d.h"
#include "shadermanager.h"
#include "meshmanager.h"

class Application
{
public:
	Application();
	Application(const Application&);
	~Application();

	bool Initialize(int screenWidth, int screenHeight, HWND hwnd);
	void Shutdown();
	bool Tick(float dt);

private:
	Direct3D* m_direct3d;
	ShaderManager* m_shaderManager;
	MeshManager* m_meshManager;
};

/*=====
GLOBALS
=====*/
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.3f;