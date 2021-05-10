#pragma once
#include "Texture.h"
#include "shaders/Shader.h"

class RenderTexture
{
private:
	Shader m_Shader;
	Texture m_FrameBufferTexture;
	unsigned int m_FBO_ID; //intermediate
	unsigned int m_MSFBO_ID; //MultiSampled FBO
	unsigned int m_RBO_ID; // RBO
	unsigned int m_VAO;
	unsigned int m_IBO;

public:
	RenderTexture(unsigned int screenWidth, unsigned int screenHeight);
	void SetShader(Shader& shader);
	void BeginRender() const;
	void EndRender() const;
	void Display();
};