#include "Particle.h"

#include "Application.h"

#include "event/game/MazeMoved.h"

Particle::Particle(float x, float y, float size, Vec2f dir, uint16_t lifetime, glm::vec4 colour)
	: x(x), y(y), m_Size(size), m_Dir(dir), m_Lifetime(lifetime), m_Age(0), m_Colour(colour)
{
}

Particle::~Particle()
{
}

void Particle::render()
{
	// Renders a rectangle on layer 4 in its position
	uint8_t layer = 4;
	Render::rectangle(x, y, 0.0f, m_Size, m_Size, m_Colour, layer);
}

void Particle::update()
{
	if(!deleteMe())
	{
		// Adds direction to x and y
		x += m_Dir.x;
		y += m_Dir.y;

		m_Age++;
	}
}

bool Particle::eventCallback(const Event::Event &e)
{
	switch(e.getType())
	{
	case Event::EventType::MazeMoved:
	{
		// If there is a maze moved event it updates its position
		const Event::MazeMovedEvent &ne = static_cast<const Event::MazeMovedEvent &>(e);
		x += ne.changeX;
		y += ne.changeY;
	}

	default:
		return false;
	}
}

void Particle::changeX(float changeBy) { x += changeBy; }
void Particle::changeY(float changeBy) { y += changeBy; }

float Particle::getX() { return x; }
float Particle::getY() { return y; }
float Particle::getSize() { return m_Size; }
bool  Particle::deleteMe() { return m_Age >= m_Lifetime; }