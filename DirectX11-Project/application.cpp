#include "application.h"

Application::Application()
{
	m_direct3d = 0;
}

Application::Application(const Application& other)
{
}

Application::~Application()
{
}

bool Application::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool success = false;

	// Create and initialize the Direct3D object.
	m_direct3d = new Direct3D;
	success = m_direct3d->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!success) {
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
	}

	return success;
}

void Application::Shutdown()
{
	// Release the Direct3D object.
	if (m_direct3d) {
		m_direct3d->Shutdown();
		delete m_direct3d;
		m_direct3d = 0;
	}
}

bool Application::Tick(float dt)
{
	bool success = true;
	// RENDER SCENE HERE
	return success;
}