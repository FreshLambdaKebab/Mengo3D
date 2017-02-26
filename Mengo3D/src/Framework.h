#ifndef _FRAMEWORK_H
#define _FRAMEWORK_H

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>

#include "Engine.h"
#include "Input.h"

class Framework
{
public:
	Framework();
	~Framework();

	bool Init();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
	bool Update();
	void InitializeWindows(int& screenWidth,int screenHeight);
	void ShutdownWindows();

	LPCSTR m_appName;
	HINSTANCE m_hInst;
	HWND m_hwnd;

	Input* m_input;
	Engine* m_engine;
};

//function prototypes
static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

//globals
static Framework* appHandle = 0;

#endif