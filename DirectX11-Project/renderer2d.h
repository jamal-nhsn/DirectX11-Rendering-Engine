#pragma once

/*======
INCLUDES
======*/
#include <vector>

#include "d3d11.h"
#include "camera2d.h"
#include "spritedata.h"
#include "vertex.h"

#define MAX_SPRITE_BATCH_SIZE (1024)

class Renderer2D
{
private:
	struct SpriteBatch
	{
		Shader* shader;
		Texture* texture;
		std::vector<Vertex2D> vertices;
	};

public:
	Renderer2D() = default;
	Renderer2D(const Renderer2D& other) = default;
	~Renderer2D();

	bool Initialize(ID3D11Device* device);

	void BeginScene(Camera2D& camera);
	SpriteData BuildSprite();
	void EndScene(ID3D11DeviceContext* deviceContext);

private:
	void const SubmitSprite(Shader* shader, Texture* texture, const SpriteData& spriteData);

private:
	ID3D11Buffer* m_vbo;
	DirectX::XMMATRIX m_cameraView;
	DirectX::XMMATRIX m_cameraProjection;
	std::vector<SpriteBatch> m_batches;

	friend SpriteData;
};