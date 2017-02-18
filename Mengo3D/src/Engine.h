#ifndef _ENGINE_H
#define _ENGINE_H

#include <Windows.h>

//globals
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class Engine
{
public:
	Engine();
	Engine(const Engine& other);
	~Engine();

	bool Initialize(int screenWidth, int screenHeight, HWND hwnd);
	void Shutdown();
	bool Update();

private:
	bool Render();
};

#endif