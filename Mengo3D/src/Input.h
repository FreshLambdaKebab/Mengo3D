#ifndef _INPUT_H
#define _INPUT_H

class Input
{
public:
	Input();
	~Input();

	void Initialize();

	void KeyDown(unsigned int input);
	void KeyUp(unsigned int input);
	bool IsKeyDown(unsigned int key);

private:
	bool m_keys[256];
};

#endif