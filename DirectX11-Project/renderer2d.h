#pragma once

/*======
INCLUDES
======*/
#include <vector>

#include "d3d11.h"
#include "camera2d.h"
#include "shader.h"
#include "texture.h"
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

	void const SubmitSprite(
		Shader* shader, Texture* texture,
		const DirectX::XMMATRIX& modelMatrix,
		int sourceX, int sourceY,
		int sourceWidth, int sourceHeight,
		const DirectX::XMFLOAT4& tint
	);

	void EndScene(ID3D11DeviceContext* deviceContext);

	void const SubmitSprite(
		Shader* shader, Texture* texture,
		float destX, float destY,
		float destWidth, float destHeight
	);
	void const SubmitSprite(
		Shader* shader, Texture* texture,
		float destX, float destY,
		float destWidth, float destHeight,
		float rotation
	);
	void const SubmitSprite(
		Shader* shader, Texture* texture,
		float destX, float destY,
		float destWidth, float destHeight,
		DirectX::XMVECTOR rotation
	);
	void const SubmitSprite(
		Shader* shader, Texture* texture,
		const DirectX::XMMATRIX& modelMatrix
	);
	void const SubmitSprite(
		Shader* shader, Texture* texture,
		float destX, float destY,
		float destWidth, float destHeight,
		int sourceX, int sourceY,
		int sourceWidth, int sourceHeight
	);
	void const SubmitSprite(
		Shader* shader, Texture* texture,
		float destX, float destY,
		float destWidth, float destHeight, 
		float rotation, 
		int sourceX, int sourceY, 
		int sourceWidth, int sourceHeight
	);
	void const SubmitSprite(
		Shader* shader, Texture* texture,
		float destX, float destY,
		float destWidth, float destHeight,
		DirectX::XMVECTOR rotation,
		int sourceX, int sourceY,
		int sourceWidth, int sourceHeight
	);
	void const SubmitSprite(
		Shader* shader, Texture* texture,
		const DirectX::XMMATRIX& modelMatrix,
		int sourceX, int sourceY,
		int sourceWidth, int sourceHeight
	);
	void const SubmitSprite(
		Shader* shader, Texture* texture,
		float destX, float destY,
		float destWidth, float destHeight,
		const DirectX::XMFLOAT4& tint
	);
	void const SubmitSprite(
		Shader* shader, Texture* texture,
		float destX, float destY,
		float destWidth, float destHeight,
		float rotation,
		const DirectX::XMFLOAT4& tint
	);
	void const SubmitSprite(
		Shader* shader, Texture* texture,
		float destX, float destY,
		float destWidth, float destHeight,
		DirectX::XMVECTOR rotation,
		const DirectX::XMFLOAT4& tint
	);
	void const SubmitSprite(
		Shader* shader, Texture* texture,
		const DirectX::XMMATRIX& modelMatrix,
		const DirectX::XMFLOAT4& tint
	);
	void const SubmitSprite(
		Shader* shader, Texture* texture,
		float destX, float destY,
		float destWidth, float destHeight,
		int sourceX, int sourceY,
		int sourceWidth, int sourceHeight,
		const DirectX::XMFLOAT4& tint
	);
	void const SubmitSprite(
		Shader* shader, Texture* texture,
		float destX, float destY,
		float destWidth, float destHeight,
		float rotation,
		int sourceX, int sourceY,
		int sourceWidth, int sourceHeight,
		const DirectX::XMFLOAT4& tint
	);
	void const SubmitSprite(
		Shader* shader, Texture* texture,
		float destX, float destY,
		float destWidth, float destHeight,
		DirectX::XMVECTOR rotation,
		int sourceX, int sourceY,
		int sourceWidth, int sourceHeight,
		const DirectX::XMFLOAT4& tint
	);

private:
	ID3D11Buffer* m_vbo;
	DirectX::XMMATRIX m_cameraView;
	DirectX::XMMATRIX m_cameraProjection;
	std::vector<SpriteBatch> m_batches;
};