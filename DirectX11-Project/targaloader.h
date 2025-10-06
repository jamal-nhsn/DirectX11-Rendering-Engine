#pragma once

/*======
INCLUDES
======*/
#include <d3d11.h>
#include <stdio.h>

#include "texture.h"

class TargaLoader
{
private:
	struct Header
	{
		unsigned char  data1[12];
		unsigned short width;
		unsigned short height;
		unsigned char  bpp;
		unsigned char  data2;
	};

public:
	TargaLoader();
	TargaLoader(const TargaLoader& other);
	~TargaLoader();

	Texture* LoadTexture(const char* filePath, ID3D11Device* device, ID3D11DeviceContext* deviceContext);
};