#include "window.h"

Window::Window()
{
	m_input = 0;
	m_application = 0;
}

Window::Window(const Window& other)
{
}

Window::~Window()
{
}

bool Window::Initialize()
{
	bool success = false;

	// Initialize Windows

	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;

	ApplicationHandle = this;
	m_hinstance = GetModuleHandle(NULL);
	m_applicationName = L"DirectX11-Project";

	// Setup the windows class with default settings.
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hinstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	RegisterClassEx(&wc);

	int screenWidth  = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	int posX = 0;
	int posY = 0;

	if (FULL_SCREEN) {
		// Set the screen to maximum size of the users desktop and 32bit.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);
	}
	else {
		screenWidth  = WINDOWED_WIDTH;
		screenHeight = WINDOWED_HEIGHT;

		// Place the window in the middle of the screen.
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth)  / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	// Create the window with the screen settings and get the handle to it.
	m_hwnd = CreateWindowEx(
		WS_EX_APPWINDOW,
		m_applicationName,
		m_applicationName,
		WS_OVERLAPPEDWINDOW,
		posX,
		posY,
		screenWidth,
		screenHeight,
		NULL,
		NULL,
		m_hinstance,
		NULL
	);

	// Bring the window up on the screen and set it as main focus.
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	// Hide the mouse cursor.
	ShowCursor(SHOW_CURSOR);

	// Create and initialize the input object.
	m_input = new Input;
	m_input->Initialize();

	// Create and initialize the application class object.
	m_application = new Application;
	success = m_application->Initialize(screenWidth, screenHeight, m_hwnd);

	return success;
}

void Window::Shutdown()
{
	// Release objects.

	if (m_application) {
		m_application->Shutdown();
		delete m_application;
		m_application = NULL;
	}
	if (m_input) {
		delete m_input;
		m_input = NULL;
	}

	// Shutdown the window.

	ShowCursor(true);

	if (FULL_SCREEN) {
		ChangeDisplaySettings(NULL, 0);
	}

	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;

	ApplicationHandle = NULL;
}

void Window::Run()
{
	// Initialize the message structure.
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	// Create timer.
	std::chrono::steady_clock::time_point begin;
	std::chrono::steady_clock::time_point end;
	float dt = 0;

	while (true) {
		// Handle the windows messages.
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		// If windows signals to end the application then exit out.
		if (msg.message == WM_QUIT) {
			break;
		}

		// Otherwise do the frame processing.
		begin = std::chrono::steady_clock::now();
		bool shouldContinue = Tick(dt);
		end = std::chrono::steady_clock::now();

		if (!shouldContinue) {
			break;
		}

		// Update delta time.
		dt = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() / 1000000.0f;
	}
}

bool Window::Tick(float dt)
{
	if (m_input->IsKeyDown(VK_ESCAPE)) {
		return false;
	}
	return m_application->Tick(dt);
}

LRESULT CALLBACK Window::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg) {
		// Check if a key has been pressed on the keyboard.
		case WM_KEYDOWN: {
			// If a key is pressed send it to the input object so it can record that state.
			m_input->KeyDown((unsigned int)wparam);
			return 0;
		}

		// Check if a key has been released on the keyboard.
		case WM_KEYUP: {
			// If a key is released then send it to the input object so it can unset the state for that key.
			m_input->KeyUp((unsigned int)wparam);
			return 0;
		}

		// Check if the window was resized.
		case WM_SIZE: {
			if (m_application && ApplicationHandle && wparam != SIZE_MINIMIZED) {
				int width = LOWORD(lparam);
				int height = HIWORD(lparam);
				m_application->Resize(width, height, m_hwnd);
			}
		}

		// Any other messages send to the default message handler as our application won't make use of them.
		default: {
			return DefWindowProc(hwnd, umsg, wparam, lparam);
		}
	}
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg) {
		// Check if the window is being destroyed.
		case WM_DESTROY: {
			PostQuitMessage(0);
			return 0;
		}

		// Check if the window is being closed.
		case WM_CLOSE: {
			PostQuitMessage(0);
			return 0;
		}

		// All other messages pass to the message handler in the system class.
		default: {
			return ApplicationHandle->MessageHandler(hwnd, umsg, wparam, lparam);
		}
	}
}