#include "Input.h"

Input::Input()
{
}

Input::~Input()
{
}

void Input::Initialize()
{
	int i;

	//initiazlie all the keys to being released and not pressed
	for(i = 0;i < 256;i++)
	{
		m_keys[i] = false;
	}
}

void Input::KeyDown(unsigned int input)
{
	//if a key has been pressed,then set that state in the key array
	m_keys[input] = true;
}

void Input::KeyUp(unsigned int input)
{
	//if a key has been released,then clear that state from the key array
	m_keys[input] = false;
}

bool Input::IsKeyDown(unsigned int key)
{
	//return what state the key is in(pressed or not pressed)
	return m_keys[key];
}
