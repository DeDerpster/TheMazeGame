#include "Spawner.h"

Spawner::Spawner()
	: x(3500.0f), y(3500.0f), m_Level(nullptr), m_Lifetime(60), m_Age(0)
{
}

Spawner::Spawner(float x, float y, Level *level)
	: x(x), y(y), m_Level(level), m_Lifetime(60), m_Age(0)
{
}

Spawner::Spawner(float x, float y, Level *level, uint16_t lifetime)
	: x(x), y(y), m_Level(level), m_Lifetime(lifetime), m_Age(0)
{
}

Spawner::~Spawner()
{
}

void Spawner::update()
{
	if(!deleteMe())
		m_Age++;
}

bool Spawner::eventCallback(const Application::Event &e)
{
	if(e.getType() == Application::EventType::mazeMovedEvent)
	{
		const Application::MazeMovedEvent &ne = static_cast<const Application::MazeMovedEvent &>(e);
		x += ne.changeX;
		y += ne.changeY;
	}
	return false;
}
