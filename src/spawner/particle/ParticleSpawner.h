#pragma once

#include "../Spawner.h"

#include "Particle.h"

#include "Direction.h"

#include <GLM.h>

class ParticleSpawner : public Spawner
{
  protected:
	std::vector<Particle> m_Particles;   // Stores the particles that are currently alive

	// Stores the spawn rate and particle life time
	uint16_t m_ParticleSpawnRate;
	uint16_t m_ParticleMinLifeTime;
	uint16_t m_ParticleMaxLifeTime;

	// These variables allow the particles to have different speeds and go in different speeds in different direction
	float m_ParticleXMinSpeed;
	float m_ParticleXMaxSpeed;
	float m_ParticleYMinSpeed;
	float m_ParticleYMaxSpeed;

	// This is for having random sizes of the particles
	float m_ParticleMinSize;
	float m_ParticleMaxSize;

	// Stores the number of particles to release in one go and the colour of the particles
	uint16_t  m_NumOfParticles;
	glm::vec4 m_Colour;

  public:
	ParticleSpawner();
	ParticleSpawner(float x, float y, Level *level);
	ParticleSpawner(float x, float y, Level *level, uint16_t mLife, uint16_t spawnRate, uint16_t minLife, uint16_t maxLife, float xMinSpeed, float xMaxSpeed, float yMinSpeed, float yMaxSpeed, float minSize, float maxSize, uint16_t groupSize, glm::vec4 colour);
	ParticleSpawner(float x, float y, Level *level, uint16_t mLife, uint16_t spawnRate, uint16_t minLife, uint16_t maxLife, Direction dir, float minSpeed, float maxSpeed, float minSize, float maxSize, uint16_t groupSize, glm::vec4 colour);
	virtual ~ParticleSpawner() override;

	virtual void render() override;
	virtual void update() override;
	virtual bool eventCallback(const Event::Event &e) override;

	glm::vec4 *            getColour();
	std::vector<Particle> *getParticles();

	virtual bool deleteMe() override;
};