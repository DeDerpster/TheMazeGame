#include "ParticleSpawner.h"

#include "RandomGen.h"

ParticleSpawner::ParticleSpawner()
	: Spawner(), m_ParticleSpawnRate(4), m_Colour({0.0f, 0.0f, 0.0f, 1.0f})
{
}

ParticleSpawner::ParticleSpawner(float x, float y, Level *level)
	: Spawner(x, y, level),
	  m_ParticleSpawnRate(4),
	  m_ParticleMinLifeTime(5),
	  m_ParticleMaxLifeTime(20),
	  m_ParticleXMinSpeed(-1.0f),
	  m_ParticleXMaxSpeed(1.0f),
	  m_ParticleYMinSpeed(-3.0f),
	  m_ParticleYMaxSpeed(3.0f),
	  m_ParticleMinSize(5.0f),
	  m_ParticleMaxSize(20.0f),
	  m_NumOfParticles(3),
	  m_Colour({0.0f, 0.0f, 0.0f, 1.0f})
{
}

ParticleSpawner::ParticleSpawner(float x, float y, Level *level, uint16_t mLife, uint16_t spawnRate, uint16_t minLife, uint16_t maxLife, float xMinSpeed, float xMaxSpeed, float yMinSpeed, float yMaxSpeed, float minSize, float maxSize, uint16_t groupSize, glm::vec4 colour)
	: Spawner(x, y, level, mLife),
	  m_ParticleSpawnRate(spawnRate),
	  m_ParticleMinLifeTime(minLife),
	  m_ParticleMaxLifeTime(maxLife),
	  m_ParticleXMinSpeed(xMinSpeed),
	  m_ParticleXMaxSpeed(xMaxSpeed),
	  m_ParticleYMinSpeed(yMinSpeed),
	  m_ParticleYMaxSpeed(yMaxSpeed),
	  m_ParticleMinSize(minSize),
	  m_ParticleMaxSize(maxSize),
	  m_NumOfParticles(groupSize),
	  m_Colour(colour)
{
}

ParticleSpawner::~ParticleSpawner()
{
}

void ParticleSpawner::update()
{
	if(m_Age != m_Lifetime)
	{
		if(m_Age % m_ParticleSpawnRate == 0)
		{
			for(int i = 0; i < m_NumOfParticles; i++)
			{
				int      xGen     = Random::getNum((int) (m_ParticleXMinSpeed * 10), (int) (m_ParticleXMaxSpeed * 10));
				int      yGen     = Random::getNum((int) (m_ParticleYMinSpeed * 10), (int) (m_ParticleYMaxSpeed * 10));
				Vec2f    dir      = {(float) xGen / 10, (float) yGen / 10};
				float    size     = (float) Random::getNum((int) (m_ParticleMinSize * 10), (int) (m_ParticleMaxSize * 10)) / 10;
				uint16_t lifetime = Random::getNum(m_ParticleMinLifeTime, m_ParticleMaxLifeTime);
				m_Particles.emplace_back(x, y, size, dir, lifetime);
			}
		}
		m_Age++;
	}

	for(auto it = m_Particles.begin(); it != m_Particles.end();)
	{
		(*it).update();
		if((*it).deleteMe())
			it = m_Particles.erase(it);
		else
			++it;
	}
}

bool ParticleSpawner::eventCallback(const Application::Event &e)
{
	for(Particle p : m_Particles)
	{
		if(p.eventCallback(e))
			return true;
	}
	return Spawner::eventCallback(e);
}

glm::vec4 *ParticleSpawner::getColour()
{
	return &m_Colour;
}

std::vector<Particle> *ParticleSpawner::getParticles()
{
	return &m_Particles;
}

bool ParticleSpawner::deleteMe()
{
	return m_Age == m_Lifetime && m_Particles.size() == 0;
}