#pragma once

/*======
INCLUDES
======*/
#include <vector>

#include "components.h"
#include "spriteanimationframe.h"

class SpriteAnimator
{
public:
	SpriteAnimator(int entityId);
	SpriteAnimator(const SpriteAnimator& other);
	~SpriteAnimator();

	int GetEntityId();

	void Update(float deltaTime);
	const SpriteAnimationFrame* GetCurrentFrame();

	void SetSpriteAnimation(const std::vector<SpriteAnimationFrame>* spriteAnimation);
	const std::vector<SpriteAnimationFrame>* GetSpriteAnimation();

	void SetTimer(float time);
	float GetTimer();

	void SetFrameNumber(int frame);
	int GetFrameNumber();

public:
	static const ComponentId ID;

private:
	int m_entityId;

	const std::vector<SpriteAnimationFrame>* m_spriteAnimation;
	float m_timer;
	int m_frame;
};