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
	glm::vec4 m_Colour;

	uint16_t m_Lifetime;
	uint16_t m_Age;

  public:
	Particle(float x, float y, float size, Vec2f dir, uint16_t lifetime, glm::vec4 colour);
	~Particle();

	virtual void render();
	virtual void update();
	virtual bool eventCallback(const Event::Event &e);

	void changeX(float changeBy);
	void changeY(float changeBy);

	float getX();
	float getY();
	float getSize();

	bool deleteMe();
};