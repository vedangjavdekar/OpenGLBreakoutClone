#pragma once
#include "Sprite.h"

class Ball :public Sprite
{
public:
	Ball();
	Ball(Texture& texture);
	Ball(Texture& texture,float radius);
	bool IsStuck;
	float Radius;
	glm::vec2 Velocity;

	void Reset(glm::vec2 position, glm::vec2 velocity);

	void ReflectBallOnBounds(unsigned int window_width, unsigned int window_height);
	void Move(float dt);
};
