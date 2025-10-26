#include "spriteanimator.h"

const ComponentId SpriteAnimator::ID = ComponentId::SpriteAnimator;

SpriteAnimator::SpriteAnimator(int entityId)
	: m_entityId(entityId)
{
	m_spriteAnimation = 0;
	m_timer = 0.0f;
	m_frame = -1;
}

SpriteAnimator::SpriteAnimator(const SpriteAnimator& other)
{
	m_entityId = other.m_entityId;
	m_spriteAnimation = other.m_spriteAnimation;
	m_timer = other.m_timer;
	m_frame = other.m_frame;
}

SpriteAnimator::~SpriteAnimator()
{
}

int SpriteAnimator::GetEntityId()
{
	return m_entityId;
}

void SpriteAnimator::Update(float dt)
{
	m_timer -= dt;
	if (m_timer <= 0.0f) {
		m_frame = (m_frame + 1) % m_spriteAnimation->size();
		m_timer += GetCurrentFrame()->duration;
	}
}

const SpriteAnimationFrame* SpriteAnimator::GetCurrentFrame()
{
	if (!m_spriteAnimation || m_frame == -1) {
		return 0;
	}

	return &(*m_spriteAnimation)[m_frame];
}

void SpriteAnimator::SetSpriteAnimation(const std::vector<SpriteAnimationFrame>* spriteAnimation)
{
	m_spriteAnimation = spriteAnimation;
	m_frame = 0;
	m_timer = GetCurrentFrame()->duration;
}

const std::vector<SpriteAnimationFrame>* SpriteAnimator::GetSpriteAnimation()
{
	return m_spriteAnimation;
}

void SpriteAnimator::SetTimer(float time)
{
	m_timer = time;
}

float SpriteAnimator::GetTimer()
{
	return m_timer;
}

void SpriteAnimator::SetFrameNumber(int frame)
{
	m_frame = frame % m_spriteAnimation->size();
}

int SpriteAnimator::GetFrameNumber()
{
	return m_frame;
}