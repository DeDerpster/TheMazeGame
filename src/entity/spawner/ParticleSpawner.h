#pragma once

#include "GLM.h"
#include "Spawner.h"

class ParticleSpawner : public Spawner
{
  protected:
	std::vector<Particle> m_Particles;

	uint16_t m_ParticleSpawnRate;
	uint16_t m_ParticleMinLifeTime;
	uint16_t m_ParticleMaxLifeTime;

	float m_ParticleXMinSpeed;
	float m_ParticleXMaxSpeed;
	float m_ParticleYMinSpeed;
	float m_ParticleYMaxSpeed;

	float m_ParticleMinSize;
	float m_ParticleMaxSize;

	uint16_t  m_NumOfParticles;
	glm::vec4 m_Colour;

  public:
	ParticleSpawner();
	ParticleSpawner(float x, float y, Level *level);
	ParticleSpawner(float x, float y, Level *level, uint16_t mLife, uint16_t spawnRate, uint16_t minLife, uint16_t maxLife, float xMinSpeed, float xMaxSpeed, float yMinSpeed, float yMaxSpeed, float minSize, float maxSize, uint16_t groupSize, glm::vec4 colour);
	virtual ~ParticleSpawner();

	virtual void update() override;
	virtual bool eventCallback(const Event::Event &e) override;

	glm::vec4 *            getColour();
	std::vector<Particle> *getParticles();

	virtual bool deleteMe() override;
};