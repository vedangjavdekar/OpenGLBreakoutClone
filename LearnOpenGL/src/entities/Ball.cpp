#include "Ball.h"

Ball::Ball()
	:IsStuck(true), Radius(20.0f), Velocity(0.0f)
{
	transform.Scale = glm::vec2(2.0f*Radius);
}

Ball::Ball(Texture& texture)
	: Sprite(texture), IsStuck(true), Radius(20.0f), Velocity(0.0f)
{
	transform.Scale = glm::vec2(2.0f * Radius);
}

Ball::Ball(Texture& texture, float radius)
	: Sprite(texture), IsStuck(true), Radius(radius), Velocity(0.0f)
{
	transform.Scale = glm::vec2(2.0f * Radius);
}

void Ball::Reset(glm::vec2 position, glm::vec2 velocity)
{
	transform.Position = position;
	this->Velocity = velocity;
	this->IsStuck = true;
}

void Ball::ReflectBallOnBounds(unsigned int window_width, unsigned int window_height)
{
	if (transform.Position.x - Radius <= 0.0f)
	{
		Velocity.x = -Velocity.x;
		transform.Position.x = Radius;
	}
	else if (transform.Position.x >= window_width - Radius)
	{
		Velocity.x = -Velocity.x;
		transform.Position.x = window_width - Radius;
	}

	if (transform.Position.y <= Radius)
	{
		Velocity.y = -Velocity.y;
		transform.Position.y = Radius;
	}
}

void Ball::Move(float dt)
{
	transform.Position += Velocity * dt;
}
