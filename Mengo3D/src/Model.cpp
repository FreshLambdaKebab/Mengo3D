#include "Model.h"

Model::Model():
	m_vertexBuffer(0),
	m_indexBuffer(0)
{
}

Model::~Model()
{
}

bool Model::Initialize(ID3D11Device * device)
{
	return false;
}

void Model::Shutdown()
{
}

void Model::Render(ID3D11DeviceContext * deviceContext)
{
}

int Model::GetIndexCount()
{
	return 0;
}

bool Model::InitializeBuffers(ID3D11Device * device)
{
	return false;
}

void Model::ShutdownBuffers()
{
}

void Model::RenderBuffers(ID3D11DeviceContext * deviceContext)
{
}
