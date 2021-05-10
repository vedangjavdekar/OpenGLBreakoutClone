#include "Renderer.h"
#include <iostream>

#include "textures/Texture.h"
#include "ResourceManager.h"

void GLClearErrors()
{
	while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error Code: " << error << " ] in the function: ["
			<< function << " ] located in the file: ["
			<< file << " ] at line "
			<< line << "."
			<< std::endl;

		return false;
	}

	return true;
}


Renderer::Renderer(Shader& shader)
	:m_Shader(shader), VAO(0), VBO(0), IBO(0)
{
	m_Shader.Bind();
	m_Shader.SetUniform1i("image", 0);
	float vertices[] = {
			 0.0f,  0.0f, 0.0f, 0.0f, //BL
			 1.0f,  0.0f, 1.0f, 0.0f, //BR
			 1.0f,  1.0f, 1.0f, 1.0f, //TR
			 0.0f,  1.0f, 0.0f, 1.0f  //TL
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};


	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(VAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, false, 4 * sizeof(float), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

Renderer::~Renderer()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &IBO);
}

void Renderer::Clear() const
{
	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::Clear(float r, float g, float b) const
{
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(r, g, b, 1.0f);
}

void Renderer::Render(Transform& transform, glm::vec4 Color,Texture* texture)
{
	m_Shader.Bind();
	m_Shader.SetUniformMat4f("model", transform.EvaluateModelMat());
	m_Shader.SetUniform4f("spriteColor", Color.r, Color.g, Color.b, Color.a);
	
	if (texture != nullptr)
	{
		texture->Bind();
	}
	else
	{
		ResourceManager::GetTexture("white").Bind();
	}

	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

