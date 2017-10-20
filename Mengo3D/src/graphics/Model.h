#ifndef _MODEL_H
#define _MODEL_H

#include <d3d11.h>
#include <DirectXMath.h>
using namespace DirectX;
#include <memory>

#include "Texture.h"

class Model
{
private:

	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
		XMFLOAT3 normal;
	};

public:
	Model();
	Model(const Model& other){}
	~Model();

	bool Initialize(ID3D11Device * device, ID3D11DeviceContext* deviceContext, char* filename);
	void Shutdown();
	void Render(ID3D11DeviceContext* deviceContext);

	int GetIndexCount()const;

	ID3D11ShaderResourceView* GetTexture()const;

private:

	bool InitializeBuffers(ID3D11Device* device);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext* deviceContext);

	bool LoadTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* filename);
	void ReleaseTexture();

	ID3D11Buffer* m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
	std::shared_ptr<Texture>m_texture;

};

#endif
