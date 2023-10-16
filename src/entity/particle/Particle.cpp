#include "Particle.h"

#include "Application.h"

Particle::Particle(int x, int y, float size, Vec2f dir, uint16_t lifetime)
	: x(x), y(y), m_Size(size), m_Dir(dir), m_Lifetime(lifetime), m_Age(0)
{
}

Particle::~Particle()
{
}

void Particle::update()
{
	if(!deleteMe())
	{
		x += m_Dir.x;
		y += m_Dir.y;

		m_Age++;
	}
}

bool Particle::eventCallback(const Application::Event &e)
{
	if(e.getType() == Application::EventType::mazeMovedEvent)
	{
		const Application::MazeMovedEvent &ne = static_cast<const Application::MazeMovedEvent &>(e);
		x += ne.changeX;
		y += ne.changeY;
	}
	return false;
}

void Particle::changeX(float changeBy) { x += changeBy; }
void Particle::changeY(float changeBy) { y += changeBy; }

float Particle::getX() { return x; }
float Particle::getY() { return y; }
float Particle::getSize() { return m_Size; }
bool  Particle::deleteMe() { return m_Age == m_Lifetime; }