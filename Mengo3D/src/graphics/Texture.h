#ifndef _TEXTURE_H
#define _TEXTURE_H

#include <d3d11.h>
#include <stdio.h>

class Texture
{
private:
	struct TargaHeader
	{
		unsigned char data1[12];
		unsigned short width;
		unsigned short height;
		unsigned char bpp;
		unsigned char data2;
	};
public:
	Texture();
	Texture(const Texture& other){}
	~Texture();

	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext,char* filename);
	void Shutdown();

	ID3D11ShaderResourceView* GetTexture()const;


private:

	//TODO:support more reading functions for differnet file types(e.g png,jpeg)
	bool LoadTarga(const char* filename, int& width, int& height);
	bool LoadPng(const char* filename,int& width, int& height);

	unsigned char* m_targaData;
	ID3D11Texture2D* m_texture;
	ID3D11ShaderResourceView* m_textureView;

};

#endif
