#include "Graphics.h"

Graphics::Graphics() :
	m_d3d(nullptr),
	m_camera(nullptr),
	m_model(nullptr),
	m_lightShader(nullptr),
	m_light(nullptr)
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
	if (!m_d3d)
	{
		return false;
	}

	//initialize the d3d object
	result = m_d3d->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, "could not fucking initialize d3d11", "error!", MB_OK);
		return false;
	}

	//create the camera object
	m_camera = std::make_unique<Camera>();
	if (!m_camera)
	{
		return false;
	}
	//set the initial position of the camera
	m_camera->SetPosition(0.0f, 0.0f, -4.0f);

	//create the model object
	m_model = std::make_shared<Model>();
	if (!m_model)
	{
		return false;
	}

	//initialize the model object
	result = m_model->Initialize(m_d3d->GetDevice(),m_d3d->GetDeviceContext(),"res/textures/stone01.tga","res/models/cube.txt");
	if (!result)
	{
		MessageBox(hwnd, "could not initailze the model object", "Error", MB_OK);
		return false;
	}

	//create the texture shader object
	m_lightShader = std::make_shared<LightShader>();
	if (!m_lightShader)
	{
		return false;
	}

	//initialize the color shader object
	result = m_lightShader->Initialize(m_d3d->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, "could not initailze the light shader object", "Error", MB_OK);
		return false;
	}

	//create the light object
	m_light = std::make_unique<Light>();
	if (!m_light)
	{
		return false;
	}

	//init the light object
	m_light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	m_light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_light->SetDirection(1.0f, 0.0f, 0.0f);

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

	//release the texture shader object
	if (m_lightShader)
	{
		m_lightShader->Shutdown();
		//delete m_lightShader;
		m_lightShader = 0;
	}

	if (m_light)
	{
		m_light = 0;
	}

	//release the model object
	if (m_model)
	{
		m_model->Shutdown();
		m_model = 0;
	}

	//release the camera object
	if (m_camera)
	{
		//delete m_camera;
		m_camera = 0;
	}
}

bool Graphics::Update()
{
	bool result;

	static float rotation = 0.0f;

	//update the rotation each frame
	rotation += static_cast<float>(3.14 * 0.007f);
	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}

	//render the graphics scene
	result = Render(rotation);
	if (!result)
	{
		return false;
	}

	return true;
}

bool Graphics::Render(float rotation)
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	bool result;
	
	//clear the buffers and begin the scene
	m_d3d->BeginScene(0.8f, 0.0f, 0.0f, 1.0f);

	//generate the view matrix based on the cameras position
	m_camera->Render();

	//get the world view and projectoin matrices from the d3d and camera objects
	m_d3d->GetWorldMatrix(worldMatrix);
	m_camera->GetViewMatrix(viewMatrix);
	m_d3d->GetProjectionMatrix(projectionMatrix);

	//rotate the world matrix by the rotation value to spin the triangle
	worldMatrix = XMMatrixRotationY(rotation);

	//put the model vertex and index buffers on the graphcis pipeline to prepare them for drawing
	m_model->Render(m_d3d->GetDeviceContext());

	//render the model using the light shader
	result = m_lightShader->Render(m_d3d->GetDeviceContext(), m_model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_model->GetTexture(),m_light->GetDirection(),m_light->GetDiffuseColor(),m_light->GetAmbientColor());
	if (!result)
	{
		return false;
	}

	//present the rendering to the screen
	m_d3d->EndScene();

	return true;
}
