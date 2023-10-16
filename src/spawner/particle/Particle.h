#pragma once

#include "2dVec.h"
#include "rendering/Renderer.h"

#include "event/Event.h"

class Particle
{
  private:
	float     x, y;
	float     m_Size;
	Vec2f     m_Dir;   // Stores the direction in vector form
	glm::vec4 m_Colour;

	// Variables for handling its death
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