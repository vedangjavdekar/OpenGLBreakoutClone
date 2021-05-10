#pragma once
#include <vector>
#include "../GameObject.h"
#include "renderer/ParticleRenderer.h"
#include "renderer/Renderer.h"
#include "Particle.h"

class ParticleEmitter :public GameObject
{
private:
	Particle particlePool[ParticleRenderer::MAX_PARTICLES];
	std::vector<Particle> m_ActiveParticles;
	ParticleRenderer* m_ParticleRenderer;

	float m_SpawnTimer;
	float m_DurationTimer;
	unsigned int currentUnusedIndex;

	Particle& GetFirstUnusedParticle();
public:
	ParticleEmitter(ParticleRenderer& renderer);
	float Duration;
	bool Looping;
	bool InLocalSpace;
	bool RandomizePosition;
	float RandomCircleRadius;
	float LifeTime;
	float SpawnRate;
	float InitialScale;
	glm::vec4 Color;
	glm::vec2 ScaleOverLifeTime; // Min Max ranges
	glm::vec2 AlphaOverLifeTime; // Min Max ranges
	glm::vec2 Velocity;
	
	Texture ParticleTexture;
	void OnSimulationSpaceChanged();
	void Update(float dt);
	void Render();
};