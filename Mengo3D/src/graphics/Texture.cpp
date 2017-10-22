#include "Texture.h"

Texture::Texture():
	m_targaData(nullptr),
	m_texture(nullptr),
	m_textureView(nullptr)
{
}

Texture::~Texture()
{
}

bool Texture::Initialize(ID3D11Device * device, ID3D11DeviceContext * deviceContext, char * filename)
{
	bool result;
	int width, height;
	D3D11_TEXTURE2D_DESC textureDesc;
	HRESULT hResult;
	unsigned int rowPitch;
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;

	//load the targa image data into memory
	result = LoadTarga(filename, width, height);
	if (!result)
	{
		return false;
	}

	//setup the description of the texture
	textureDesc.Height = height;
	textureDesc.Width = width;
	textureDesc.MipLevels = 0;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

	//create the empty texture
	hResult = device->CreateTexture2D(&textureDesc, 0, &m_texture);
	if (FAILED(hResult))
	{
		return false;
	}

	//set the row pitch of the targa data
	rowPitch = (width * 4) * sizeof(unsigned char);

	//copy the targa image data into the texture
	deviceContext->UpdateSubresource(m_texture, 0, 0, m_targaData, rowPitch, 0);

	//setup the shader resource view description
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = -1;

	//create the shader resource view for the texture
	hResult = device->CreateShaderResourceView(m_texture, &srvDesc, &m_textureView);
	if (FAILED(result))
	{
		return false;
	}

	//generate mip maps for this texture
	deviceContext->GenerateMips(m_textureView);

	//release the targa image data now that the image data has been loaded into the texture
	delete[] m_targaData;
	m_targaData = 0;

	return true;
}

void Texture::Shutdown()
{
	//release the texture view resource
	if (m_textureView)
	{
		m_textureView->Release();
		m_textureView = 0;
	}

	//release the texture
	if (m_texture)
	{
		m_texture->Release();
		m_texture = 0;
	}

	//release the targa data
	if (m_targaData)
	{
		delete[] m_targaData;
		m_targaData = 0;
	}

}

ID3D11ShaderResourceView * Texture::GetTexture() const
{
	return m_textureView;
}

bool Texture::LoadTarga(const char * filename, int & width, int & height)
{
	int error, bpp, imageSize, index, i, j, k;
	FILE* pFile;
	unsigned int count;
	TargaHeader targaFileHeader;
	unsigned char* targaImage;

	//open the targa file for reading in binary
	error = fopen_s(&pFile, filename, "rb");
	if (error != 0)
	{
		return false;
	}

	//read in the file header
	count = static_cast<unsigned int>(fread(&targaFileHeader, sizeof(TargaHeader), 1, pFile));
	if (count != 1)
	{
		return false;
	}

	//get the important information from the header
	height = static_cast<int>(targaFileHeader.height);
	width = static_cast<int>(targaFileHeader.width);
	bpp = static_cast<int>(targaFileHeader.bpp);

	//check and make sure its 32bit and not 24
	if (bpp != 32)
	{
		return false;
	}

	//calculate the size of the 32 bit image data
	imageSize = width * height * 4;

	//allocate memory of the targa image data
	targaImage = new unsigned char[imageSize];
	if (!targaImage)
	{
		return false;
	}

	//read in the targa image data
	count = static_cast<unsigned int>(fread(targaImage, 1, imageSize, pFile));
	if (count != imageSize)
	{
		return false;
	}

	//close the file
	error = fclose(pFile);
	if (error != 0)
	{
		return false;
	}

	//allocate memory for the targa destination data
	m_targaData = new unsigned char[imageSize];
	if (!m_targaData)
	{
		return false;
	}

	index = 0;

	//initialize the index into the targa data
	k = (width * height * 4) - (width * 4);

	//now copy the targa image data into the targa destination array in the correct order since the targa format is stored upside down
	for (j = 0;j < height; j++)
	{
		for (i = 0; i < width; i++)
		{
			m_targaData[index + 0] = targaImage[k + 2];  // Red.
			m_targaData[index + 1] = targaImage[k + 1];  // Green.
			m_targaData[index + 2] = targaImage[k + 0];  // Blue
			m_targaData[index + 3] = targaImage[k + 3];  // Alpha

			//increment the indexes into the targa data
			k += 4;
			index += 4;
		}
		//set the targa image data back to the preceeding row at the beginning of the column since its reading it in upside down
		k -= (width * 8);
	}

	//release the targa image data now that it was copied into the destination array
	delete[] targaImage;
	targaImage = 0;

	return true;
}

bool Texture::LoadPng(const char * filename, int & width, int & height)
{
	//TODO:load a png...
	return false;
}
