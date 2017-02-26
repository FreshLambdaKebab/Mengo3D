#include "Engine.h"

Engine::Engine()
{
	m_d3d = 0;
}

Engine::Engine(const Engine & other)
{
}

Engine::~Engine()
{
}

bool Engine::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result = true;

	//create the d3d object
	m_d3d = new D3DManager();
	if (!m_d3d)
	{
		return false;
	}

	//initialize the d3d object
	m_d3d->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, "coult not initialize direct3d", "error!", MB_OK);
		return false;
	}
	
	return true;
}

void Engine::Shutdown()
{
	//release the d3d obejct
	if (m_d3d)
	{
		m_d3d->Shutdown();
		delete m_d3d;
		m_d3d = 0;
	}
}

bool Engine::Update()
{
	bool result;

	//render the scene each frame
	result = Render();
	if (!result)
	{
		return false;
	}

	return true;
}

bool Engine::Render()
{
	//clear all the buffers and beign the scene
	m_d3d->BeginScene(1.0f,0.0f,1.0f,1.0f);

	//present the scene
	m_d3d->EndScene();
	return true;
}
