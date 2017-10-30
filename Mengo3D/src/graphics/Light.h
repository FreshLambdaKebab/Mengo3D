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

	void SetAmbientColor(float r, float g, float b, float a);
	void SetDiffuseColor(float r, float g, float b, float a);
	void SetDirection(float x, float y, float z);
	void SetSpecularColor(float r, float g, float b, float a);
	void SetSpecularPower(float specularPower);

	const XMFLOAT4& GetAmbientColor()const { return m_ambientColor; }
	const XMFLOAT4& GetDiffuseColor()const { return m_diffuseColor; }
	const XMFLOAT3& GetDirection()const { return m_direction; }
	const XMFLOAT4& GetSpecularColor()const { return m_specularColor; }
	const float& GetSpecularPower()const { return m_specularPower; }

private:
	XMFLOAT4 m_ambientColor;
	XMFLOAT4 m_diffuseColor;
	XMFLOAT3 m_direction;
	XMFLOAT4 m_specularColor;
	float m_specularPower;

};

#endif
