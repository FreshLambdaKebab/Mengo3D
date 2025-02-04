#include "Light.h"

Light::Light()
{
}

Light::~Light()
{
}

void Light::SetAmbientColor(float r, float g, float b, float a)
{
	m_ambientColor = XMFLOAT4(r, g, b, a);
}

void Light::SetDiffuseColor(float r, float g, float b, float a)
{
	m_diffuseColor = XMFLOAT4(r, g, b, a);
}

void Light::SetDirection(float x, float y, float z)
{
	m_direction = XMFLOAT3(x, y, z);
}

void Light::SetSpecularColor(float r, float g, float b, float a)
{
	m_specularColor = XMFLOAT4(r, g, b, a);
}

void Light::SetSpecularPower(float specularPower)
{
	m_specularPower = specularPower;
}
