#ifndef _MODEL_H
#define _MODEL_H

#include <d3d11.h>
#include <DirectXMath.h>
using namespace DirectX;

class Model
{
private:

	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT4 color;
	};

public:
	Model();
	Model(const Model& other){}
	~Model();

	bool Initialize(ID3D11Device* device);
	void Shutdown();
	void Render(ID3D11DeviceContext* deviceContext);

	int GetIndexCount()const;

private:

	bool InitializeBuffers(ID3D11Device* device);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext* deviceContext);

	ID3D11Buffer* m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;

};

#endif
