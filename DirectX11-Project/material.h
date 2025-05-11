#pragma once

/*======
INCLUDES
======*/
#include <d3d11.h>

class Material
{
public:
	Material();
	Material(const Material&);
	~Material();

	bool Initialize();
};