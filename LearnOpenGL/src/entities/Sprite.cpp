#include "Sprite.h"

Sprite::Sprite()
	:m_Color(1.0f,1.0f,1.0f,1.0f)
{
}

Sprite::Sprite(Texture& texture)
	:GameObject(),m_Texture(texture),m_Color(1.0f)
{
}

Sprite::Sprite(const std::string& name, Texture& texture)
	:GameObject(name),m_Texture(texture),m_Color(1.0f)
{
}


void Sprite::SetTexture(Texture& texture)
{
	m_Texture = texture;
}

void Sprite::Render(Renderer& renderer)
{
	renderer.Render(this->transform, this->m_Color, &this->m_Texture);
}
