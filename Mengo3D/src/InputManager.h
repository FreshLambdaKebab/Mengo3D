#ifndef _INPUT_MANAGER_H
#define _INPUT_MANAGER_H

class InputManager
{
public:
	InputManager();
	InputManager(const InputManager& other){}
	~InputManager();

	void Initialize();

	void KeyDown(unsigned int key);
	void KeyUp(unsigned int key);
	bool IsKeyDown(unsigned int key);

private:
	bool m_keys[256];
};

#endif
