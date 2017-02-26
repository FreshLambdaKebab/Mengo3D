#include "Framework.h"

Framework::Framework()
{
	m_input = 0;
	m_engine = 0;
}

Framework::~Framework()
{
}

bool Framework::Init()
{
	int screenWidth, screenHeight;
	bool result;

	//init the width and height to 0 before sending them into the function
	screenWidth = 0;
	screenHeight = 0;

	//initialize the windows api
	InitializeWindows(screenWidth, screenHeight);

	//create and then initialize the input object,this will be used to handle reading input from the user
	m_input = new Input();
	if (!m_input)
	{
		return false;
	}
	m_input->Initialize();

	//create and initialize the engine object, this handles drawing all the graphics and engine capabilities
	m_engine = new Engine();
	if (!m_engine)
	{
		return false;
	}
	result = m_engine->Initialize(screenWidth,screenHeight,m_hwnd);
	if (!result)
	{
		return false;
	}

	return true;
}

void Framework::Shutdown()
{
	//release the engine object
	if (m_engine)
	{
		m_engine->Shutdown();
		delete m_engine;
		m_engine = 0;
	}

	//release teh input object
	if (m_input)
	{
		delete m_input;
		m_input = 0;
	}

	ShutdownWindows();
}

void Framework::Run()
{
	MSG msg;
	bool done, result;

	//initilize the MSG struct
	ZeroMemory(&msg, sizeof(MSG));

	done = false;
	//keep looping until there is a quit message
	while (!done)
	{
		//handle the windows messages
		if (PeekMessage(&msg, m_hwnd, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		//if windows or the user signals to quit the application then exit
		if (msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			//otherwise update the application
			result = Update();
			if (!result)
			{
				done = true;
			}
		}
	}
}

LRESULT Framework::MessageHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		//check if any key has been pressed
	case WM_KEYDOWN:
	{
		//if a key has been pressed then send it to the input object so it can record that state
		m_input->KeyDown((unsigned int)wParam);
		return 0;
	}break;
	//check if a key has been released 
	case WM_KEYUP:
	{
		//if a key was released send it to the input object so it can unset the state for that key
		m_input->KeyUp((unsigned int)wParam);
		return 0;
	}break;
	//send any other messages to the default message handler as we dont need use of them
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}

bool Framework::Update()
{
	bool result;

	//check if the user has pressed escape and wants to qiut
	if (m_input->IsKeyDown(VK_ESCAPE))
	{
		return false;
	}

	//do the update processing for the engine object
	result = m_engine->Update();
	if (!result)
	{
		return false;
	}

	return true;
}

void Framework::InitializeWindows(int & screenWidth, int screenHeight)
{
	WNDCLASSEX wc;
	DEVMODE dmc;
	int posX, posY;

	//get an external pointer to this object
	appHandle = this;

	//get the inst of the app
	m_hInst = GetModuleHandle(NULL);

	//give the application a name
	m_appName = "The fucking DirectX window";

	//setup the window class struct with all default settings
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hInst;
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = 0;
	wc.lpszClassName = m_appName;
	wc.cbSize = sizeof(WNDCLASSEX);

	//register the windows class
	RegisterClassEx(&wc);

	//determine the res of the clients desktop screen
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	//setup the screen settings depenending if the user is running fullscreen or windowed mode
	if (FULL_SCREEN)
	{
		//if fullscreen is chosen,set to the entire dimensions of the users desktop and also 32 bit
		memset(&dmc, 0, sizeof(dmc));
		dmc.dmSize = sizeof(dmc);
		dmc.dmPelsWidth = (unsigned long)screenWidth;
		dmc.dmPelsHeight = (unsigned long)screenHeight;
		dmc.dmBitsPerPel = 32;
		dmc.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		//change the display settings to fullscreen
		ChangeDisplaySettings(&dmc, CDS_FULLSCREEN);
		
		//set the pos of the window to the top left corner
		posX = 0;
		posY = 0;
	}
	else
	{
		//if windowed then set to desired width and height (800x600)
		screenWidth = 800;
		screenHeight = 600;

		//place window in the middle of screen
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;

	}

	RECT windowRect = { 0,0,screenWidth,screenHeight };//set the size but not the position
	AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, false);

	//create the window with all the screen settings,then get the handle to it
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_appName, m_appName, WS_OVERLAPPEDWINDOW | WS_POPUP, posX, posY,
							windowRect.right - windowRect.left, windowRect.bottom - windowRect.top,0, 0, m_hInst, 0);

	//bring the window to the screen and set it as the main focus
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);
}

void Framework::ShutdownWindows()
{
	//fix the display settings if leaving fullscreen mode
	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(0, 0);
	}

	//remove the window
	DestroyWindow(m_hwnd);
	m_hwnd = 0;

	//remove the applicatoin instance
	UnregisterClass(m_appName, m_hInst);
	m_hInst = 0;

	appHandle = 0;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		//chekc if the window is being destroyed
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
		break;
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
		break;
	default:
		return appHandle->MessageHandler(hwnd, message, wParam, lParam);
	}
}
