#pragma once
#include <GL/glew.h>
#include <iostream>
#include "entities/Trasnform.h"
#include "shaders/Shader.h"

#define ASSERT(x) if(!(x)) __debugbreak();

#define GLCall(x) GLClearErrors();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearErrors();
bool GLLogCall(const char* function, const char* file, int line);

class Texture;

class Renderer
{
protected:
	Shader m_Shader;
	unsigned int VAO;
	unsigned int VBO;
	unsigned int IBO;
public:
	Renderer(Shader& shader);
	~Renderer();
	void Clear() const;
	void Clear(float r, float g, float b) const;
	virtual void Render(Transform& transform, glm::vec4 Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), Texture* texture=nullptr);
};