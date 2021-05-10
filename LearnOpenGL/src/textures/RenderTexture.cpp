#include "RenderTexture.h"
#include "GL/glew.h"
#include <iostream>
#include "renderer/Renderer.h"

RenderTexture::RenderTexture(unsigned int screenWidth, unsigned int screenHeight)
{
	GLCall(glGenFramebuffers(1, &m_MSFBO_ID));
	GLCall(glGenFramebuffers(1, &m_FBO_ID));
	GLCall(glGenRenderbuffers(1, &m_RBO_ID));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_MSFBO_ID));
	GLCall(glBindRenderbuffer(GL_RENDERBUFFER, m_RBO_ID));
	GLCall(glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_RGB, screenWidth, screenHeight)); // allocate storage for render buffer object
	GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_RBO_ID));

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::POSTPROCESSOR: Failed to initialize MSFBO" << std::endl;

	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_FBO_ID));
	m_FrameBufferTexture.Generate(screenWidth, screenHeight);
	GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_FrameBufferTexture.GetTextureID(), 0));

	std::cout << "Render Texture Dimensions: (" << m_FrameBufferTexture.GetWidth() << "," << m_FrameBufferTexture.GetHeight()<<")"<< std::endl;

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::POSTPROCESSOR:: Failed to initialize FBO!" << std::endl;
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));

	float vertices[] = {
			 -1.0f, -1.0f, 0.0f, 0.0f, //BL
			  1.0f, -1.0f, 1.0f, 0.0f, //BR
			  1.0f,  1.0f, 1.0f, 1.0f, //TR
			 -1.0f,  1.0f, 0.0f, 1.0f  //TL
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	GLCall(glGenVertexArrays(1, &m_VAO));

	unsigned int VBO;
	GLCall(glGenBuffers(1, &VBO));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
	GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));

	GLCall(glBindVertexArray(m_VAO));

	GLCall(glEnableVertexAttribArray(0));
	GLCall(glVertexAttribPointer(0, 4, GL_FLOAT, false, 4 * sizeof(float), (void*)0));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));

	GLCall(glGenBuffers(1, &m_IBO));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	GLCall(glBindVertexArray(0));

}

void RenderTexture::SetShader(Shader& shader)
{
	m_Shader = shader;
	m_Shader.Bind();
	m_Shader.SetUniform1i("image", 0);
}

void RenderTexture::BeginRender() const
{
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_MSFBO_ID));
	GLCall(glClearColor(1.0f, 0.0f, 0.0f, 1.0f));
	GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void RenderTexture::EndRender() const
{
	GLCall(glBindFramebuffer(GL_READ_FRAMEBUFFER, m_MSFBO_ID));
	GLCall(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_FBO_ID));
	unsigned int width = m_FrameBufferTexture.GetWidth(), height = m_FrameBufferTexture.GetHeight();
	GLCall(glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void RenderTexture::Display()
{
	GLCall(glClearColor(1.0f, 1.0f, 1.0f, 1.0f));
	GLCall(glClear(GL_COLOR_BUFFER_BIT));
	m_Shader.Bind();
	m_FrameBufferTexture.Bind();
	GLCall(glBindVertexArray(m_VAO));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO));
	GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
}
