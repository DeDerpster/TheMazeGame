#pragma once

#include "Renderer.h"
#include "Utils.h"

#include "Event.h"

class Particle
{
  private:
	float x, y;
	float m_Size;
	Vec2f m_Dir;

	uint16_t m_Lifetime;
	uint16_t m_Age;

  public:
	Particle(int x, int y, float size, Vec2f dir, uint16_t lifetime);
	~Particle();

	virtual void update();
	virtual bool eventCallback(const Application::Event &e);

	void changeX(float changeBy);
	void changeY(float changeBy);

	float getX();
	float getY();
	float getSize();

	bool deleteMe();
};