#ifndef _LIGHT_H
#define _LIGHT_H

#include <DirectXMath.h>
using namespace DirectX;

class Light
{
public:
	Light();
	Light(const Light& other){}
	~Light();

	void SetDiffuseColor(float r, float g, float b, float a);
	void SetDirection(float x, float y, float z);

	const XMFLOAT4& GetDiffuseColor()const { return m_diffuseColor; }
	const XMFLOAT3& GetDirection()const { return m_direction; }

private:
	XMFLOAT4 m_diffuseColor;
	XMFLOAT3 m_direction;

};

#endif
