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
		Texture* texture;
		std::vector<Vertex2D> vertices;
	};

public:
	Render2DSystem();
	Render2DSystem(const Render2DSystem& other);
	~Render2DSystem();

	bool Initialize(ID3D11Device* device);
	void Shutdown();
	void Update(ID3D11DeviceContext* deviceContext, Scene* scene);

private:
	void CreateBatches(Scene* scene, float viewWidth, float viewHeight);
	void RenderBatches(ID3D11DeviceContext* deviceContext, Camera2D& camera);

private:
	std::vector<Batch> m_batches;
	ID3D11Buffer* m_vbo;
};