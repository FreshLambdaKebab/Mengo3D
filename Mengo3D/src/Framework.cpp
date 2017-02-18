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
	return false;
}

void Framework::Shutdown()
{
}

void Framework::Run()
{
}

LRESULT Framework::MessageHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return LRESULT();
}

bool Framework::Update()
{
	return false;
}

void Framework::InitializeWindows(int & screenWidth, int screenHeight)
{
}

void Framework::ShutdownWindows()
{
}
