#pragma once
#include "Trasnform.h"
#include <string>

class GameObject
{
public:
	GameObject();
	GameObject(const std::string& name);

	std::string name;
	Transform transform;

	virtual void Update(float dt) {}
};