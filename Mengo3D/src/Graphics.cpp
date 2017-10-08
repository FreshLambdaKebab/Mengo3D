#include "Graphics.h"

Graphics::Graphics() :
	m_d3d(nullptr)
{
}

Graphics::~Graphics()
{
}

bool Graphics::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;

	//create the d3d object
	m_d3d = std::make_unique<D3DManager>();
	//m_d3d = new D3DManager();
	if (!m_d3d)
	{
		return false;
	}

	//initialize the d3d object
	result = m_d3d->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, "could not initialize d3d11", "error!", MB_OK);
		return false;
	}

	return true;
}

void Graphics::Shutdown()
{
	//release the d3d object
	if (m_d3d)
	{
		m_d3d->Shutdown();
		//delete m_d3d;
		m_d3d = nullptr;
	}
}

bool Graphics::Update()
{
	bool result;

	//render the graphics scene
	result = Render();
	if (!result)
	{
		return false;
	}

	return true;
}

bool Graphics::Render()
{
	//clear the buffers and begin the scene
	m_d3d->BeginScene(1.0f, 0.0f, 0.0f, 1.0f);

	m_d3d->EndScene();

	return true;
}
