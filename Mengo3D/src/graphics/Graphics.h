#ifndef _GRAPHICS_H
#define _GRAPHICS_H

#include "D3DManager.h"
#include "../Constants.h"
#include "../Camera.h"
#include "Model.h"
#include "ColorShader.h"
#include <memory>

class Graphics
{
public:
	Graphics();
	Graphics(const Graphics& other){}
	~Graphics();

	bool Initialize(int screenWidth, int screenHeight, HWND hwnd);
	void Shutdown();
	bool Update();

private:
	bool Render();

	//D3DManager* m_d3d;
	std::unique_ptr<D3DManager>m_d3d;///attempting to use smart pointers instead of raw pointers
	std::unique_ptr<Camera>m_camera;
	std::shared_ptr<Model>m_model;
	std::shared_ptr<ColorShader>m_colorShader;
};

#endif
