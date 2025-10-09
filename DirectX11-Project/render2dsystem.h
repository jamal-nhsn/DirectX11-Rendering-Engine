#pragma once

/*======
INCLUDES
======*/
#include <unordered_map>

#include "scene.h"
#include "direct3d.h"
#include "sprite.h"
#include "vertex.h"

#define MAX_SPRITE_BATCH_SIZE (1024)

class Render2DSystem
{
private:
	struct Batch
	{
		Shader* shader;
		std::vector<ID3D11ShaderResourceView*> textures;
		std::vector<Vertex2D> vertices;
	};

public:
	Render2DSystem();
	Render2DSystem(const Render2DSystem& other);
	~Render2DSystem();

	void Update(Direct3D* direct3d, Scene* scene);

private:
	bool Initialize(ID3D11Device* device);
	void CreateBatches(Scene* scene);
	void RenderBatches(Direct3D* direct3d, Camera2D& camera);
	void ClearBatches();

private:
	std::vector<Batch> m_batches;
	ID3D11Buffer* m_vbo;
};