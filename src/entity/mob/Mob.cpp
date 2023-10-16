#include "Mob.h"
#include "Tile.h"

#include <math.h>

Mob::Mob()
	: Entity(0.0f, 0.0f, nullptr, {{(float) Tile::TILE_SIZE * 0.37f, (float) -Tile::TILE_SIZE * 0.04f}, {(float) Tile::TILE_SIZE * 0.6f, (float) Tile::TILE_SIZE * 0.2f}}), m_Speed(7.0f), isMoving(false), m_Dir(Direction::SOUTH)
{
}

Mob::Mob(float x, float y)
	: Entity(x, y, nullptr, {{(float) Tile::TILE_SIZE * 0.37f, (float) -Tile::TILE_SIZE * 0.04f}, {(float) Tile::TILE_SIZE * 0.6f, (float) Tile::TILE_SIZE * 0.2f}}), m_Speed(7.0f), isMoving(false), m_Dir(Direction::SOUTH)
{
}

Mob::Mob(float x, float y, Level *level)
	: Entity(x, y, level, {{(float) Tile::TILE_SIZE * 0.37f, (float) -Tile::TILE_SIZE * 0.04f}, {(float) Tile::TILE_SIZE * 0.6f, (float) Tile::TILE_SIZE * 0.2f}}), m_Speed(7.0f), isMoving(false), m_Dir(Direction::SOUTH)
{
}

Mob::~Mob()
{
	for(Item *item : inventory)
		delete item;
}

void Mob::move(float xa, float ya)
{
	if(!isGhost)
	{
		if(m_Level->collisionDetection(x + xa, y, m_CollisionBox))
			xa = 0;
		if(m_Level->collisionDetection(x, y + ya, m_CollisionBox))
			ya = 0;
		if((xa == 0 && ya == 0) || m_Level->collisionDetection(x + xa, y + ya, m_CollisionBox))
		{
			isMoving = false;
			return;
		}
	}
	isMoving = true;
	x += xa;
	y += ya;
	if(fabs(xa) > fabs(ya))
	{
		if(ya < 0)
			m_Dir = Direction::SOUTH;
		else if(ya > 0)
			m_Dir = Direction::NORTH;
		if(xa < 0)
			m_Dir = Direction::WEST;
		else if(xa > 0)
			m_Dir = Direction::EAST;
	}
	else
	{
		if(xa < 0)
			m_Dir = Direction::WEST;
		else if(xa > 0)
			m_Dir = Direction::EAST;
		if(ya < 0)
			m_Dir = Direction::SOUTH;
		else if(ya > 0)
			m_Dir = Direction::NORTH;
	}
}

void Mob::move(Vec2f ratio)
{
	if(ratio.y == 0)
		move(ratio.x * m_Speed, 0.0f);
	else if(ratio.x == 0)
		move(0.0f, ratio.y * m_Speed);
	else
	{
		float speedSquared = m_Speed * m_Speed;
		float sum          = std::fabs(ratio.x) + std::fabs(ratio.y);
		float sumSquared   = sum * sum;
		float timesBy      = std::sqrt((sumSquared * speedSquared) / (ratio.x * ratio.x + ratio.y * ratio.y));
		move((ratio.x * timesBy) / sum, (ratio.y * timesBy) / sum);
	}
}

void Mob::pickUp(Item *item)
{
	inventory.push_back(item);
}