#include "window.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	// Create the window object.
	Window* window = new Window;

	// Initialize and run the window object.
	bool success = window->Initialize();
	if (success) {
		window->Run();
	}

	// Shutdown and release the window object.
	window->Shutdown();
	delete window;
	window = 0;

	return 0;
}