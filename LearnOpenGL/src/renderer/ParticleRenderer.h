#pragma once
#include <vector>
#include "GL/glew.h"
#include "shaders/Shader.h"
#include "entities/particles/Particle.h"
#include "textures/Texture.h"

class ParticleRenderer
{
private:
	Shader m_Shader;
	unsigned int VAO, VBO, IBO;

public:
	static const int MAX_PARTICLES = 100;

	std::vector<float> dataFrame;
	std::vector<unsigned int> indices;
	
	ParticleRenderer(Shader& shader);
	~ParticleRenderer();

	void AddParticle(Particle& particle);
	void ClearData();
	void UpdateData();
	void Render(Texture* texture);
};