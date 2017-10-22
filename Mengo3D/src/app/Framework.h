#ifndef _FRAME_WORK_H
#define _FRAME_WORK_H

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <memory>

#include "../graphics/Graphics.h"
#include "../IO/InputManager.h"

class Framework
{
public:
	Framework();
	Framework(const Framework& other);
	~Framework();

	bool Initialize();
	void Run();
	void Shutdown();

	LRESULT CALLBACK MessageHandler(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

private:

	bool Update();//gets called every frame
	void InitializeWindows(int& screenWidth, int& screenHeight);
	void ShutdownWindows();

	LPCSTR m_appName;
	HINSTANCE m_hInst;
	HWND m_hwnd;

	Graphics* m_graphics;
	InputManager* m_input;

};

//prototypes
static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

//globals
static Framework* ApplicationHandle = 0;

#endif //_FRAME_WORK_H
