#include "Model.h"

Model::Model() :
	m_vertexBuffer(0),
	m_indexBuffer(0),
	m_vertexCount(0),
	m_indexCount(0),
	m_texture(nullptr),
	m_model(nullptr)
{
}

Model::~Model()
{
}

bool Model::Initialize(ID3D11Device * device, ID3D11DeviceContext* deviceContext, char* textureFilename, char* modelFilename)
{
	bool result;

	//load the model data
	result = LoadModel(modelFilename);
	if (!result)
	{
		return false;
	}

	//initialize the vertex and index buffers
	result = InitializeBuffers(device);
	if (!result)
	{
		return false;
	}

	//load the texture for this model
	result = LoadTexture(device, deviceContext, textureFilename);
	if (!result)
	{
		return false;
	}

	return true;
}

void Model::Shutdown()
{
	//shutdown the vertex and idnex buffers
	ShutdownBuffers();

	//release the model texture
	ReleaseTexture();

	//release the model data
	ReleaseModel();
}

void Model::Render(ID3D11DeviceContext * deviceContext)
{
	//put the vertex and index buffers on the graphics pipeline to prepare the for drawing
	RenderBuffers(deviceContext);
}

int Model::GetIndexCount() const
{
	return m_indexCount;
}

ID3D11ShaderResourceView * Model::GetTexture() const
{
	return m_texture->GetTexture();
}

bool Model::InitializeBuffers(ID3D11Device * device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	//create the vertex array.
	vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	//create the index array
	indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		return false;
	}

	//load the vertex array and index array with data
	for (int i = 0; i < m_vertexCount; i++)
	{
		vertices[i].position = XMFLOAT3(m_model[i].x, m_model[i].y, m_model[i].z);
		vertices[i].texture = XMFLOAT2(m_model[i].tu, m_model[i].tv);
		vertices[i].normal = XMFLOAT3(m_model[i].nx, m_model[i].ny, m_model[i].nz);

		indices[i] = i;
	}
	

	//set up the description of the static vertex buffer
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	//give the subresource structure a pointer to the vertex data
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	//now create the vertex buffer
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	//set up the description of the static index buffer
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	//give the subresource structure a pointer to the index data
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	//create the index buffer
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	//release the arrays since the vertex and index buffers have been created
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}

void Model::ShutdownBuffers()
{
	//release the index buffer
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	//release the vertex buffer
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}
}

void Model::RenderBuffers(ID3D11DeviceContext * deviceContext)
{
	unsigned int stride, offset;

	//set the buffer stride and offset
	stride = sizeof(VertexType);
	offset = 0;

	//set the vertex buffer to active in the input assembler so it can be rendered 
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	//set the index buffer to active in the input assembler so it can be rendered
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT,0);

	//set the primitive type that should be rendered from the vertex buffer, triangles for our sake
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

bool Model::LoadTexture(ID3D11Device * device, ID3D11DeviceContext * deviceContext, char * filename)
{
	bool result;

	//create the texture object
	m_texture = std::make_shared<Texture>();
	if (!m_texture)
	{
		return false;
	}

	//initialize the texture object
	result = m_texture->Initialize(device, deviceContext, filename);
	if (!result)
	{
		return false;
	}

	return true;
}

void Model::ReleaseTexture()
{
	//release the texture object
	if (m_texture)
	{
		m_texture->Shutdown();
		m_texture = 0;
	}
}

bool Model::LoadModel(const char * filename)
{
	std::ifstream file;
	char input;

	//open the model file
	file.open(filename);

	//if the file failed to open, then quit
	if (file.fail())
	{
		return false;
	}

	//read up to the value of vertex count
	file.get(input);
	while (input != ':')
	{
		file.get(input);
	}

	//read in the vertex count
	file >> m_vertexCount;

	//set the num of indices to be the same as the vertex count
	m_indexCount = m_vertexCount;

	//create the model using the vertex count that was read in
	m_model = new ModelType[m_vertexCount];
	if (!m_model)
	{
		return false;
	}

	//read to the beginning of the data
	file.get(input);
	while (input != ':')
	{
		file.get(input);
	}
	file.get(input);
	file.get(input);

	//read in the vertex data
	for (int i = 0; i < m_vertexCount; i++)
	{
		file >> m_model[i].x >> m_model[i].y >> m_model[i].z;
		file >> m_model[i].tu >> m_model[i].tv;
		file >> m_model[i].nx >> m_model[i].ny >> m_model[i].nz;
	}

	file.close();

	return true;
}

void Model::ReleaseModel()
{
	if (m_model)
	{
		delete[] m_model;
		m_model = 0;
	}
}
