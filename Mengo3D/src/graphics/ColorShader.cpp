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
	bool result;

	//initialize the vertex and pixel shaders
	result = InitializeShader(device, hwnd, L"res/shaders/color_shader.vs", L"res/shaders/color_shader.ps");
	if (!result)
	{
		return false;
	}

	return true;
}

void ColorShader::Shutdown()
{
	//shutdown the vertex and pixel shaders and alo other related objects
	ShutdownShader();
}

bool ColorShader::Render(ID3D11DeviceContext * deviceContext, int indexCount, XMMATRIX& world,XMMATRIX& view, XMMATRIX& projection)
{
	bool result;

	//set the shader parameters that it will use for rendering
	result = SetShaderParameters(deviceContext, world, view, projection);
	if (!result)
		return false;

	//render the prepared buffers with the shader
	RenderShader(deviceContext, indexCount);
}

bool ColorShader::InitializeShader(ID3D11Device * device, HWND hwnd, WCHAR * vsFilename, WCHAR * psFilename)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;

	//init the pointer this funciton will use to null
	errorMessage = 0;
	vertexShaderBuffer = 0;
	pixelShaderBuffer = 0;

	//compile the vertex shader code
	result = D3DCompileFromFile(vsFilename, NULL, NULL, "ColorVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
		&vertexShaderBuffer, &errorMessage);
	if (FAILED(result))
	{
		//if the shader failed to compile it should have writen something to the error message
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, vsFilename);
		}
		//if there was  nothing in the error message then it simply could not find the shader file itself
		else
		{
			MessageBox(hwnd, (LPCSTR)vsFilename, "Missing Shader File", MB_OK);
		}

		return false;
	}

	//compile the pixel shader code
	result = D3DCompileFromFile(psFilename, NULL, NULL, "ColorPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
		&pixelShaderBuffer, &errorMessage);
	if (FAILED(result))
	{
		//if the shader failed to compile it should have writen something to the error message
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, psFilename);
		}
		//if there was nothing in the error message then it simply could not find the file itself
		else
		{
			MessageBox(hwnd, (LPCSTR)psFilename, "Missing Shader File", MB_OK);
		}

		return false;
	}

	//create the vertex shader from the buffer
	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), 0, &m_vertexShader);
	if (FAILED(result))
	{
		return false;
	}

	//create the pixel shader from the buffer
	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), 0, &m_pixelShader);
	if (FAILED(result))
	{
		return false;
	}

	//create the vertex input layout description\
	this setup needs to match the VertexType stucture in the ModelClass and in the shader
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "COLOR";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	//count the elements in the layout
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	//create the vertex input layout
	result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(), &m_layout);
	if (FAILED(result))
	{
		return false;
	}

	//release the vertex shader buffer and pixel shader buffer since they are no longer needed
	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;

	//setup the description of the dynamic matrix constant buffer that is in the vertex shader
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	//create the constant buffer so we can access the vertex shader constant buffer from within this class
	result = device->CreateBuffer(&matrixBufferDesc, 0, &m_matrixBuffer);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void ColorShader::ShutdownShader()
{
	//release the matrix constant buffer
	if (m_matrixBuffer)
	{
		m_matrixBuffer->Release();
		m_matrixBuffer = 0;
	}

	//release the layout
	if (m_layout)
	{
		m_layout->Release();
		m_layout = 0;
	}

	//release the pixel shader
	if (m_pixelShader)
	{
		m_pixelShader->Release();
		m_pixelShader = 0;
	}

	//release the vertex shader.
	if (m_vertexShader)
	{
		m_vertexShader->Release();
		m_vertexShader = 0;
	}
}

void ColorShader::OutputShaderErrorMessage(ID3D10Blob * errorMessage, HWND hwnd, WCHAR * fileName)
{
	char* compileErrors;
	unsigned long bufferSize, i;
	std::ofstream fout;

	//get a pointer to the error message text buffer
	compileErrors = static_cast<char*>(errorMessage->GetBufferPointer());

	//get the length of the message
	bufferSize = errorMessage->GetBufferSize();

	//open a file to write the text message to
	fout.open("shader-error.txt");

	//write the error message
	for (i = 0; i < bufferSize; i++)
	{
		fout << compileErrors[i];
	}

	//close the file
	fout.close();

	//release the error message
	errorMessage->Release();
	errorMessage = 0;

	//notify the user to check the file with a messgebox
	MessageBox(hwnd, "Error compiling shader!", "error!", MB_OK);
}

bool ColorShader::SetShaderParameters(ID3D11DeviceContext * deviceContext,XMMATRIX& worldMatrix,XMMATRIX& viewMatrix,XMMATRIX& projectionMatrix)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	unsigned int bufferNumber;

	worldMatrix = XMMatrixTranspose(worldMatrix);
	viewMatrix = XMMatrixTranspose(viewMatrix);
	projectionMatrix = XMMatrixTranspose(projectionMatrix);

	//lock the constant buffer so it can be written to
	result = deviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	//get a pointer to the data in the constant buffer
	dataPtr = static_cast<MatrixBufferType*>(mappedResource.pData);

	//copy the matrices into the constant buffer
	dataPtr->world = worldMatrix;
	dataPtr->view = viewMatrix;
	dataPtr->projection = projectionMatrix;

	//unlock the constant buffer
	deviceContext->Unmap(m_matrixBuffer, 0);

	//set the number of constant buffers in the vertex shader
	bufferNumber = 0;

	//finally set the constant buffer in the vertex shader with the updated values
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);

	return true;
}

void ColorShader::RenderShader(ID3D11DeviceContext * deviceContext, int indexCount)
{
	//set the vertex input layout
	deviceContext->IASetInputLayout(m_layout);

	//set the vertex and pixel shaders that will be used to render this triangle
	deviceContext->VSSetShader(m_vertexShader, 0, 0);
	deviceContext->PSSetShader(m_pixelShader, 0, 0);

	//render the triangle
	deviceContext->DrawIndexed(indexCount, 0, 0);
}
