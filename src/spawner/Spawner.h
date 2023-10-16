#pragma once

#include "event/Event.h"

class Level;
class Spawner
{
  protected:
	float  x, y;      // Stores the position of the spawner
	Level *m_Level;   // Stores the current level

	// Two variables to determine when the spawner should be destroyed
	uint16_t m_Lifetime;
	uint16_t m_Age;

  public:
	Spawner();
	Spawner(float x, float y, Level *level);
	Spawner(float x, float y, Level *level, uint16_t lifetime);
	virtual ~Spawner();

	virtual void render() = 0;
	virtual void update();
	virtual bool eventCallback(const Event::Event &e);

	virtual bool deleteMe() { return m_Age >= m_Lifetime; }

	float getX() { return x; }
	float getY() { return y; }
};