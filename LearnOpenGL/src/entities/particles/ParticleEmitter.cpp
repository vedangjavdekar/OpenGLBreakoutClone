#include "ParticleEmitter.h"

float Lerp(float min, float max, float interpolation);

Particle& ParticleEmitter::GetFirstUnusedParticle()
{
	int currIndex = currentUnusedIndex;
	while (particlePool[currentUnusedIndex].LifeTime > 0)
	{
		currentUnusedIndex++;
		if (currentUnusedIndex == ParticleRenderer::MAX_PARTICLES)
		{
			currentUnusedIndex = 0;
		}
		if (currentUnusedIndex == currIndex)
		{
			break;
		}
	}
	if (currentUnusedIndex == currIndex)
	{
		currentUnusedIndex = 0;
		return particlePool[currentUnusedIndex];
	}
	else
	{
		return particlePool[currentUnusedIndex];
	}
}

ParticleEmitter::ParticleEmitter(ParticleRenderer& renderer)
	:m_ParticleRenderer(&renderer), Duration(0), Looping(false), LifeTime(1.0f), SpawnRate(0.2f), Color(1.0f, 1.0f, 1.0f, 1.0f), InitialScale(50.0f),
	ScaleOverLifeTime(1.0f, 1.0f), AlphaOverLifeTime(1.0f, 1.0f),
	Velocity(0.0f, 0.0f),
	currentUnusedIndex(0),
	m_SpawnTimer(0), m_DurationTimer(0),
	InLocalSpace(false),
	RandomizePosition(false),RandomCircleRadius(5.0f)
{

}


void ParticleEmitter::OnSimulationSpaceChanged()
{
	if (InLocalSpace)
	{
		for (int i = 0; i < m_ActiveParticles.size(); i++)
		{
			m_ActiveParticles[i].transform.Position -= transform.Position;
		}
	}
	else
	{
		for (int i = 0; i < m_ActiveParticles.size(); i++)
		{
			m_ActiveParticles[i].transform.Position += transform.Position;
		}
	}
}

void ParticleEmitter::Update(float dt)
{
	if (m_DurationTimer <= Duration)
	{
		for (int i = 0; i < m_ActiveParticles.size(); i++)
		{
			if (m_ActiveParticles[i].LifeTime <= 0.0f)
			{
				m_ActiveParticles.erase(m_ActiveParticles.begin() + i);
			}
			else
			{
				m_ActiveParticles[i].transform.Position += m_ActiveParticles[i].Velocity * dt;
				float interpolation = m_ActiveParticles[i].LifeTime / this->LifeTime;
				m_ActiveParticles[i].transform.Scale = glm::vec2(InitialScale * Lerp(ScaleOverLifeTime.x, ScaleOverLifeTime.y, interpolation));
				m_ActiveParticles[i].Color.a = Lerp(AlphaOverLifeTime.x, AlphaOverLifeTime.y, interpolation);
				m_ActiveParticles[i].LifeTime -= dt;
			}
		}
		m_SpawnTimer += dt;
		if (m_SpawnTimer > SpawnRate)
		{
			m_SpawnTimer = 0;
			Particle p = GetFirstUnusedParticle();
			p.LifeTime = LifeTime;
			p.Color = Color;
			p.Velocity = Velocity;
			if (!InLocalSpace)
			{
				p.transform.Position = transform.Position;
			}
			if (RandomizePosition)
			{
				float angle = glm::radians((float)(rand() % 360));
				p.transform.Position += RandomCircleRadius * glm::vec2(std::cos(angle), std::sin(angle));
			}
			p.transform.Scale = glm::vec2(InitialScale);
			m_ActiveParticles.push_back(p);
		}
	}

	m_DurationTimer += dt;
	if (m_DurationTimer > Duration)
	{
		if (Looping)
		{
			m_DurationTimer = 0;
		}
	}
}

void ParticleEmitter::Render()
{
	m_ParticleRenderer->ClearData();
	for (auto& particle : m_ActiveParticles)
	{
		glm::vec2 localPos = particle.transform.Position;
		if (InLocalSpace)
		{
			particle.transform.Position = transform.Position + localPos;
		}
		m_ParticleRenderer->AddParticle(particle);
		if (InLocalSpace)
		{
			particle.transform.Position = localPos;
		}
	}
	m_ParticleRenderer->UpdateData();
	m_ParticleRenderer->Render(&ParticleTexture);
}


float Lerp(float min, float max, float interpolation)
{
	min = std::max(min, 0.0f);
	max = std::min(max, 1.0f);
	interpolation = std::max(std::min(interpolation, 1.0f), 0.0f);
	return (1 - interpolation) * min + interpolation * max;
}