#pragma once
#include "Sprite.h"

class Brick :public Sprite
{
public:
	bool Destroyed;
	bool IsSolid;
};