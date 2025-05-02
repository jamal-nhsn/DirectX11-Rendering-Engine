#pragma once

/*=======================
PRE-PROCESSING DIRECTIVES
=======================*/
#define WIN32_LEAN_AND_MEAN

/*======
INCLUDES
======*/
#include <windows.h>

#include "input.h"
#include "application.h"

class Window
{
public:
	Window();
	Window(const Window& other);
	~Window();

	bool Initialize();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);

private:
	bool Tick();

private:
	LPCWSTR   m_applicationName;
	HINSTANCE m_hinstance;
	HWND      m_hwnd;

	Input*       m_input;
	Application* m_application;
};

/*=================
FUNCTION PROTOTYPES
=================*/
static LRESULT CALLBACK WndProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);

/*=====
GLOBALS
=====*/
static Window* ApplicationHandle = NULL;
const int WINDOWED_WIDTH = 800;
const int WINDOWED_HEIGHT = 600;
const bool SHOW_CURSOR = false;