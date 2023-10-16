#include "Follower.h"
#include "Application.h"
#include "Tile.h"

#include <thread>

Follower::Follower()
	: m_Name("Bob")
{
}

Follower::Follower(float x, float y)
	: Mob(x, y), m_Name("Bob")
{
}

Follower::Follower(float x, float y, Level *level)
	: Mob(x, y, level), m_Name("Bob")
{
}

Follower::Follower(float x, float y, Level *level, uint16_t spriteID)
	: Mob(x, y, level, spriteID), m_Name("Bob")
{
}

Follower::~Follower()
{
}

void Follower::findPath()
{
	Vec2f dest  = {following->getX(), following->getY()};
	Vec2f start = {x, y};
	if(!m_Level)
		Log::critical("Level is null", LOGINFO);
	std::vector<Vec2f> *path = m_Level->getPath(start, dest, m_CollisionBox);

	if(path->size() == 0)
	{
		Log::error("Path is empty!", LOGINFO);
		return;
	}

	float availiableDist = m_Speed;
	while(availiableDist > 0.0f && path->size() > 0)
	{
		float distToNext = distBetweenVec2f({x, y}, path->back());
		Vec2f distVec    = {path->back().x - x, path->back().y - y};
		if(distToNext < availiableDist)
		{
			availiableDist -= distToNext;
			move(distVec.x, distVec.y);
			path->pop_back();
		}
		else
		{
			availiableDist = 0.0f;
			float timesBy  = m_Speed / distToNext;
			move(distVec.x * timesBy, distVec.y * timesBy);
		}
	}

	findingPath = false;
	delete path;
}

void Follower::update()
{
	if(following && !findingPath)
	{
		float xDif        = following->getX() - x;
		float yDif        = following->getY() - y;
		float minDistAway = (Tile::TILE_SIZE / 3) * 2;
		if(xDif < -minDistAway || xDif > minDistAway || yDif < -minDistAway || yDif > minDistAway)
		{
			findingPath = true;
			findPath();
		}
		else
			isMoving = false;
	}

	Mob::update();
}

void Follower::render()
{
	Mob::render();
}

#ifdef DEBUG
void Follower::imGuiRender()
{
}
#endif

bool Follower::eventCallback(const Application::Event &e)
{
	return false;
}