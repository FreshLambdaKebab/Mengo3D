#include "Framework.h"


Framework::Framework():
	m_graphics(0),
	m_input(0)
{
}

Framework::Framework(const Framework & other)
{
}

Framework::~Framework()
{
}

bool Framework::Initialize()
{
	int screenWidth, screenHeight;
	bool result;

	screenWidth = screenHeight = 0;

	//init the windows api
	InitializeWindows(screenWidth, screenHeight);

	//create and initialize the input object, this will be used to handle reading keyboard input from the user
	m_input = new InputManager();
	if (!m_input)
		return false;

	m_input->Initialize();

	//create and init the graphics object, this will handle rendering graphics for this app
	m_graphics = new Graphics();
	if (!m_graphics)
		return false;

	result = m_graphics->Initialize(screenWidth,screenHeight,m_hwnd);
	if (!result)
		return false;


	return true;
}

void Framework::Run()
{
	MSG msg;
	bool done, result;

	//initialize the message struct
	ZeroMemory(&msg, sizeof(MSG));

	//loop until there is a quit message from the window or the user
	done = false;
	while (!done)
	{
		//hanlde windows messages
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		//if windows signals to end the application then quit out
		if (msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			//otherwise update everythign
			result = Update();
			if (!result)
			{
				done = true;
			}

		}

	}
}

void Framework::Shutdown()
{
	//release the graphics object
	if (m_graphics)
	{
		//m_graphics->Shutdown();
		delete m_graphics;
		m_graphics = 0;
	}

	//release the input object
	if (m_input)
	{
		delete m_input;
		m_input = 0;
	}

	ShutdownWindows();
}

LRESULT Framework::MessageHandler(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		//check if a key has been pressed on the keyboard
	case WM_KEYDOWN:
	{
		//if a key is pressed send it to the input object so it can record that state
		m_input->KeyDown((unsigned int)wParam);
		return 0;
	}break;
	//check if a key has been released
	case WM_KEYUP:
	{
		//if a key is released send it to the input object so it can unset taht state for that key
		m_input->KeyUp((unsigned int)wParam);
		return 0;
	}break;
	//any other messages send to the default message handler as the app wont make use of them.
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
}

bool Framework::Update()
{
	bool result;

	//check if the user pressed escape and wants to quit this shit
	if (m_input->IsKeyDown(VK_ESCAPE))
	{
		return false;
	}

	//do the frame processing for the graphics object
	result = m_graphics->Update();
	if (!result)
	{
		return false;
	}

	return true;
}

void Framework::InitializeWindows(int & screenWidth, int & screenHeight)
{
	WNDCLASSEX wc;
	DEVMODE dmc;
	int posX, posY;

	//get an external pointer to this object
	ApplicationHandle = this;

	//get the instance of this application
	m_hInst = GetModuleHandle(NULL);

	//give teh app a name
	m_appName = "The fucking window title used crackwhore";

	//setup the windows class with the default settings
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hInst;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_appName;
	wc.cbSize = sizeof(WNDCLASSEX);

	//register the window class
	RegisterClassEx(&wc);

	//get the res of the clients desktop screen
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	RECT windowRect;

	//setup screen settings based on if the user is running fullscreen or windowed mode
	if (FULL_SCREEN)
	{
		//if fullscreen set to the size of users desktop(in 32bit)
		memset(&dmc, 0, sizeof(dmc));
		dmc.dmSize = sizeof(dmc);
		dmc.dmPelsWidth = (unsigned long)screenWidth;
		dmc.dmPelsHeight = (unsigned long)screenHeight;
		dmc.dmBitsPerPel = 32;
		dmc.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		//change display settings to fullscreen
		ChangeDisplaySettings(&dmc, CDS_FULLSCREEN);

		//set the position of the window to the top left
		posX = 0;
		posY = 0;
	}
	else
	{
		//otherwise if not fullscreen, set the window to a desired height
		screenWidth = 800;
		screenHeight = 600;

		//calculate the size of the client rect
		windowRect = { 0,0,screenWidth,screenHeight };
		AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_POPUP, false);

		//place the window in the middle of the screen
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	//create the window with the settings and get the handle to it
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_appName, m_appName, WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_POPUP,
		posX, posY, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top, 0, 0, m_hInst, 0);

	//bring the window onto the screen and set it as the main focus
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

}

void Framework::ShutdownWindows()
{
	//show the mouse cursor(if it was hidden)
	ShowCursor(true);

	//fix display settings if leaving fullscreen mode
	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	//remove the window
	DestroyWindow(m_hwnd);
	m_hwnd = 0;

	//remove teh app instance
	UnregisterClass((LPCSTR)m_appName, m_hInst);
	m_hInst = 0;

	//release the pointer to this class
	ApplicationHandle = 0;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		//check if the window is being destroyed
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
		//check if the window is beign closed
		case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}
		// all other messages pass to the message handler in the framework class
		default:
		{
			return ApplicationHandle->MessageHandler(hwnd, message, wParam, lParam);
		}
	}
}
