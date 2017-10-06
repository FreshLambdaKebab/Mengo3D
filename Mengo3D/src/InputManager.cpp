#include "InputManager.h"

InputManager::InputManager()
{
}

InputManager::~InputManager()
{
}

void InputManager::Initialize()
{
	int i;

	//init all the keys to being released and not pressed
	for (int i = 0; i < 256; i++)
	{
		m_keys[i] = false;
	}
}

void InputManager::KeyDown(unsigned int key)
{
	//if a key is pressed,save taht key in teh key array
	m_keys[key] = true;
}

void InputManager::KeyUp(unsigned int key)
{
	//if a key is released, clear that state from teh key array
	m_keys[key] = false;
}

bool InputManager::IsKeyDown(unsigned int key)
{
	//return the state of the key(pressed/not pressed)
	return m_keys[key];
}
