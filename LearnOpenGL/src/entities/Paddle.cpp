#include "Paddle.h"

Paddle::Paddle(Texture& texture)
	:Sprite(texture)
{
}

void Paddle::ClampXPosition(float min, float max)
{
	transform.Position.x = std::max(transform.Position.x, min + transform.Scale.x * 0.5f);
	transform.Position.x = std::min(transform.Position.x, max - transform.Scale.x * 0.5f);
}

void Paddle::Update(float dt)
{
	transform.Position += Velocity * dt;
}

void Paddle::Reset(glm::vec2 position, glm::vec2 velocity)
{
	transform.Position = position;
	Velocity = velocity;
}
