#pragma once

/*======
INCLUDES
======*/
#include <vector>

#include "components.h"
#include "../Renderer/spriteanimation.h"

class SpriteAnimator
{
public:
	SpriteAnimator(int entityId);
	SpriteAnimator(const SpriteAnimator& other);
	~SpriteAnimator();

	int GetEntityId();

	void Update(float deltaTime);
	const Engine::SpriteAnimationFrame* GetCurrentFrame();

	void SetSpriteAnimation(const Engine::SpriteAnimation* spriteAnimation);
	const Engine::SpriteAnimation* GetSpriteAnimation();

	void SetTimer(float time);
	float GetTimer();

	void SetFrameNumber(int frame);
	int GetFrameNumber();

public:
	static const ComponentId ID;

private:
	int m_entityId;

	const Engine::SpriteAnimation* m_spriteAnimation;
	float m_timer;
	int m_frame;
};