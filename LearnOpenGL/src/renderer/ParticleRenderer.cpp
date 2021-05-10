#include "ParticleRenderer.h"
#include "ResourceManager.h"
#include "renderer/Renderer.h"

ParticleRenderer::ParticleRenderer(Shader& shader)
	:m_Shader(shader), VAO(0), VBO(0), IBO(0)
{
	m_Shader.Bind();
	m_Shader.SetUniform1i("image", 0);
	ClearData();
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * 4 * 8 * sizeof(float), dataFrame.data(), GL_DYNAMIC_DRAW);

	glBindVertexArray(VAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, false, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, false, 8 * sizeof(float), (void*)(4 * sizeof(float)));

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, MAX_PARTICLES * 6 * sizeof(unsigned int), indices.data(), GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

ParticleRenderer::~ParticleRenderer()
{
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &IBO);
	glDeleteVertexArrays(1, &VAO);
}

void ParticleRenderer::AddParticle(Particle& particle)
{
	glm::mat4 model = particle.transform.EvaluateModelMat();
	glm::vec4 bottomLeft = model * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	glm::vec4 bottomRight = model * glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	glm::vec4 topRight = model * glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
	glm::vec4 topLeft = model * glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);

	dataFrame.insert(dataFrame.end(), { bottomLeft.x, bottomLeft.y, 0.0f, 0.0f, particle.Color.r, particle.Color.g, particle.Color.b, particle.Color.a });
	dataFrame.insert(dataFrame.end(), { bottomRight.x, bottomRight.y, 1.0f, 0.0f, particle.Color.r, particle.Color.g, particle.Color.b, particle.Color.a });
	dataFrame.insert(dataFrame.end(), { topRight.x, topRight.y, 1.0f, 1.0f, particle.Color.r, particle.Color.g, particle.Color.b, particle.Color.a });
	dataFrame.insert(dataFrame.end(), { topLeft.x, topLeft.y, 0.0f, 1.0f, particle.Color.r, particle.Color.g, particle.Color.b, particle.Color.a });
	unsigned int startIndex = indices.size() > 0 ? 4 * indices.size() / 6 : 0;
	indices.insert(indices.end(), { startIndex,startIndex + 1,startIndex + 2,startIndex + 2,startIndex + 3,startIndex });
}

void ParticleRenderer::ClearData()
{
	dataFrame.clear();
	indices.clear();
}

void ParticleRenderer::UpdateData()
{
	
	GLCall(glBindVertexArray(VAO));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
	GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, dataFrame.size() * sizeof(float), &dataFrame[0]));
	GLCall(glBindVertexArray(0));

	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO));
	GLCall(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indices.size() * sizeof(unsigned int), &indices[0]));


}

void ParticleRenderer::Render(Texture* texture)
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	m_Shader.Bind();
	if (texture != nullptr)
	{
		texture->Bind();
	}
	else {
		ResourceManager::GetTexture("white").Bind();
	}
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
