#pragma once
#include "../GameObject.h"

class Particle: public GameObject
{
public:
	Particle();
	float LifeTime;
	glm::vec4 Color;
	glm::vec2 Velocity;
};