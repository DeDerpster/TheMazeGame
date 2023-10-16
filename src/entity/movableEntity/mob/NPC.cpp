#include "NPC.h"

#include <thread>
#include <vector>

#include "Application.h"
#include "FireStaff.h"
#include "Room.h"
#include "Tile.h"
#include "Utils.h"

NPC::NPC()
	: m_Name("Bob"), following(nullptr), attacking(nullptr), findingPath(false), isRunningAway(false)
{
	generateInventory();
}

NPC::NPC(float x, float y)
	: Mob(x, y), m_Name("Bob"), following(nullptr), attacking(nullptr), findingPath(false), isRunningAway(false)
{
	generateInventory();
}

NPC::NPC(float x, float y, Level *level)
	: Mob(x, y, level), m_Name("Bob"), following(nullptr), attacking(nullptr), findingPath(false), isRunningAway(false)
{
	generateInventory();
}

NPC::NPC(float x, float y, Level *level, uint16_t spriteID)
	: Mob(x, y, level, spriteID), m_Name("Bob"), following(nullptr), attacking(nullptr), findingPath(false), isRunningAway(false)
{
	generateInventory();
}

NPC::~NPC()
{
}

void NPC::generateInventory()
{
	pickUp(new FireStaff());
}
void NPC::findPath()
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

void NPC::follow()
{
	if(!following)
	{
		Log::warning("Trying to follow a nullptr!");
		return;
	}
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

void NPC::attack()
{
	if(!attacking)
	{
		Log::warning("Trying to attack a nullptr!");
		return;
	}
	if(!isRunningAway && getStamina() == 0)
		isRunningAway = true;
	else if(isRunningAway && getStamina() > 3 * getMaxStamina() / 4)
		isRunningAway = false;

	bool moveInX = false;
	if(fabs(x - attacking->getX()) < fabs(y - attacking->getY()))
		moveInX = true;

	Vec2f ratio = {0, 0};
	if(x < attacking->getX() && x + m_Speed < attacking->getX())
	{
		if(moveInX)
			ratio.x += 1.0f;
		else if(abs(x - attacking->getX()) < 200)
			ratio.x -= 1.0f;
		else if(abs(x - attacking->getX()) > 250)
			ratio.x += 1.0f;
	}
	else if(x > attacking->getX() && x + m_Speed > attacking->getX())
	{
		if(moveInX)
			ratio.x -= 1.0f;
		else if(abs(x - attacking->getX()) < 200)
			ratio.x += 1.0f;
		else if(abs(x - attacking->getX()) > 250)
			ratio.x -= 1.0f;
	}
	else
	{
		if(y > attacking->getY())
			m_Dir = Direction::south;
		else
			m_Dir = Direction::north;
		m_Weapons[m_CurrentWeapon]->attack(m_Level, *this, m_Dir, true);
	}

	if(y < attacking->getY() && y + m_Speed < attacking->getY())
	{
		if(!moveInX)
			ratio.y += 1.0f;
		else if(abs(y - attacking->getY()) < 200)
			ratio.y -= 1.0f;
		else if(abs(y - attacking->getY()) > 250)
			ratio.y += 1.0f;
	}
	else if(y > attacking->getY() && y + m_Speed > attacking->getY())
	{
		if(!moveInX)
			ratio.y -= 1.0f;
		else if(abs(y - attacking->getY()) < 200)
			ratio.y += 1.0f;
		else if(abs(y - attacking->getY()) > 250)
			ratio.y -= 1.0f;
	}
	else
	{
		if(x > attacking->getX())
			m_Dir = Direction::west;
		else
			m_Dir = Direction::east;
		m_Weapons[m_CurrentWeapon]->attack(m_Level, *this, m_Dir, true);
	}

	if(ratio.x != 0.0f || ratio.y != 0.0f)
	{
		if(isRunningAway)   // TODO: PLS CHANGE THIS
			move({-ratio.x, -ratio.y});
		else
			move(ratio);
	}
	else
	{
		if(isRunningAway)
			move({-1.0f, -1.0f});
		else
			isMoving = false;
	}
}

void NPC::update()
{
	if(following && !findingPath)
	{
		follow();
	}
	else if(attacking && (int) attacking->getX() / (Tile::TILE_SIZE * ROOM_SIZE) == (int) x / (Tile::TILE_SIZE * ROOM_SIZE) && (int) attacking->getY() / (Tile::TILE_SIZE * ROOM_SIZE) == (int) y / (Tile::TILE_SIZE * ROOM_SIZE))
	{
		attack();
	}
	else
		isMoving = false;

	Mob::update();
}

void NPC::render()
{
	Mob::render();
}

#ifdef DEBUG
void NPC::imGuiRender()
{
}
#endif

bool NPC::eventCallback(const Event::Event &e)
{
	return Mob::eventCallback(e);
}