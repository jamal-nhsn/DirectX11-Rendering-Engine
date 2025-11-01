#pragma once

/*======
INCLUDES
======*/
#include <vector>

#include "d3d11.h"
#include "camera2d.h"
#include "vertex.h"
#include "spritedata.h"
#include "textdata.h"

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

	void const SubmitSprite(const SpriteData& spriteData, const DirectX::XMMATRIX& modelMatrix);
	void const SubmitSprite(const SpriteData& spriteData, float centerX, float centerY, float width, float height);
	void const SubmitSprite(const SpriteData& spriteData, float centerX, float centerY, float width, float height, float rotationDeg);
	void const SubmitSprite(const SpriteData& spriteData, float centerX, float centerY, float width, float height, DirectX::XMVECTOR rotationQuaternion);

	void const SubmitText(const TextData& textData, const DirectX::XMMATRIX& modelMatrix);

	void EndScene(ID3D11DeviceContext* deviceContext);

private:
	ID3D11Buffer* m_vbo;
	ID3D11Buffer* m_ibo;

	DirectX::XMMATRIX m_cameraView;
	DirectX::XMMATRIX m_cameraProjection;
	std::vector<SpriteBatch> m_batches;

	size_t m_lastBatchIndex;
};