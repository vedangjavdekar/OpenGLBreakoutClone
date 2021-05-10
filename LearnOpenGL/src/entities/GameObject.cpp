#include "GameObject.h"

GameObject::GameObject()
	:name("GameObject"),transform()
{
}

GameObject::GameObject(const std::string& name)
	:name(name),transform()
{
}
