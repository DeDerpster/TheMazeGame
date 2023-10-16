#include "Spawner.h"

#include "layer/level/Level.h"

#include "event/game/MazeMoved.h"

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
	// This increases its age if its not already dead
	if(!deleteMe())
		m_Age++;
}

bool Spawner::eventCallback(const Event::Event &e)
{
	switch(e.getType())
	{
	case Event::EventType::MazeMoved:
	{
		// If there is a maze moved event it updates its coords
		const Event::MazeMovedEvent &ne = static_cast<const Event::MazeMovedEvent &>(e);
		x += ne.changeX;
		y += ne.changeY;
	}

	default:
		return false;
	}
}
