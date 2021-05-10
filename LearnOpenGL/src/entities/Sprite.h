#pragma once
#include "textures/Texture.h"
#include "GameObject.h"
#include "renderer/Renderer.h"

class Sprite : public GameObject
{
public:
	Sprite();
	Sprite(Texture& texture);
	Sprite(const std::string& name,Texture& texture);
	
	inline glm::vec4 GetColor()const 
	{
		return m_Color;
	}

	inline void SetColor(glm::vec3 color)
	{
		m_Color = glm::vec4(color, 1.0f);
	}

	inline void SetColor(glm::vec4 color)
	{
		m_Color = color;
	}

	inline Texture GetTexture() const
	{
		return m_Texture;
	}
	
	void SetTexture(Texture& texture);
	void Render(Renderer& renderer);
private:
	Texture m_Texture;
	glm::vec4 m_Color;
};
