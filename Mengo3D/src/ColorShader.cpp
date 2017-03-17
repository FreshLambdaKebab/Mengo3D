#include "ColorShader.h"

ColorShader::ColorShader() :
	m_vertexShader(0),
	m_pixelShader(0),
	m_layout(0),
	m_matrixBuffer(0)
{
}

ColorShader::~ColorShader()
{
}

bool ColorShader::Initialize(ID3D11Device * device, HWND hwnd)
{
	return false;
}

void ColorShader::Shutdown()
{
}

bool ColorShader::Render(ID3D11DeviceContext * deviceContext, int indexCount, XMMATRIX & worldMatrix, XMMATRIX & viewMatrix, XMMATRIX & projectionMatrix)
{
	return false;
}

bool ColorShader::InitializeShader(ID3D11Device * device, HWND hwnd, WCHAR * vsFilename, WCHAR * psFilename)
{
	return false;
}

void ColorShader::ShutdownShader()
{
}

void ColorShader::OutputShaderErrorMessage(ID3D10Blob * errorMessage, HWND hwnd, WCHAR * shaderFilename)
{
}

bool ColorShader::SetShaderParameters(ID3D11DeviceContext * deviceContext, XMMATRIX & worldMatrix, XMMATRIX & viewMatrix, XMMATRIX & projectionMatrix)
{
	return false;
}

void ColorShader::RenderShader(ID3D11DeviceContext * deviceContext, int indexCount)
{
}
