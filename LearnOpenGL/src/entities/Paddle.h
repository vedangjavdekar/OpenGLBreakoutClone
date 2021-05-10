#pragma once
#include "Sprite.h"

class Paddle : public Sprite
{
public:
	Paddle(Texture& texture);
	glm::vec2 Velocity;

	void ClampXPosition(float min, float max);

	virtual void Update(float dt)override;
	void Reset(glm::vec2 position, glm::vec2 velocity);
};