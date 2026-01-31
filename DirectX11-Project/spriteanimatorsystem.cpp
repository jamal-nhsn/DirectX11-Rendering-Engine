#include "pch.h"
#include "spriteanimatorsystem.h"
#include "shader.h"

SpriteAnimatorSystem::SpriteAnimatorSystem()
{
};

SpriteAnimatorSystem::SpriteAnimatorSystem(const SpriteAnimatorSystem& other)
{
}

SpriteAnimatorSystem::~SpriteAnimatorSystem()
{
}

void SpriteAnimatorSystem::Update(Scene* scene, float dt)
{
	std::vector<SpriteAnimator>* spriteAnimators = scene->GetComponents<SpriteAnimator>();

	for (SpriteAnimator& spriteAnimator : (*spriteAnimators)) {

		spriteAnimator.Update(dt);
		const SpriteAnimationFrame* frame = spriteAnimator.GetCurrentFrame();

		if (!frame || !scene->HasComponent<Sprite>(spriteAnimator.GetEntityId())) {
			continue;
		}

		Sprite& sprite = scene->GetComponent<Sprite>(spriteAnimator.GetEntityId());

		sprite.SetTexture(frame->texture);
		sprite.SetSourceX(frame->sourceX);
		sprite.SetSourceY(frame->sourceY);
		sprite.SetWidth(frame->width);
		sprite.SetHeight(frame->height);	
	}
}