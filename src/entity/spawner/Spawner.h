#pragma once

#include "Level.h"

#include "Particle.h"
#include "Renderer.h"

class Spawner
{
  protected:
	float  x, y;
	Level *m_Level;

	uint16_t m_Lifetime;
	uint16_t m_Age;

  public:
	Spawner();
	Spawner(float x, float y, Level *level);
	Spawner(float x, float y, Level *level, uint16_t lifetime);
	~Spawner();

	virtual void update();
	virtual bool eventCallback(const Application::Event &e);

	virtual bool deleteMe() { return m_Age == m_Lifetime; }
};