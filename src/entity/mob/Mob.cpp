#include "Mob.h"
#include "Tile.h"

Mob::Mob()
	: Entity(), m_Speed(7.0f), isMoving(false), m_Dir(Direction::SOUTH)
{
}

Mob::Mob(float x, float y)
	: Entity(x, y), m_Speed(7.0f), isMoving(false), m_Dir(Direction::SOUTH)
{
}

Mob::Mob(float x, float y, Level *level)
	: Entity(x, y, level), m_Speed(7.0f), isMoving(false), m_Dir(Direction::SOUTH)
{
}

Mob::~Mob()
{
}

void Mob::move(float xa, float ya)
{
	if(!isGhost)
	{
		if(collision(xa, 0))
			xa = 0;
		if(collision(0, ya))
			ya = 0;
		if(xa == 0 && ya == 0)
		{
			isMoving = false;
			return;
		}
		if(collision(xa, ya))
		{
			isMoving = false;
			return;
		}
	}
	isMoving = true;
	x += xa;
	y += ya;
	if(xa < 0)
		m_Dir = Direction::WEST;
	else if(xa > 0)
		m_Dir = Direction::EAST;
	if(ya < 0)
		m_Dir = Direction::SOUTH;
	else if(ya > 0)
		m_Dir = Direction::NORTH;
}

bool Mob::collision(float xa, float ya)
{
	int  tileX      = (x + xa + Tile::TILE_SIZE * 0.37f) / Tile::TILE_SIZE;
	int  tileY      = (y + ya - Tile::TILE_SIZE * 0.04f) / Tile::TILE_SIZE;
	bool lowerBound = m_Level->getTile(tileX, tileY)->isSolid();

	tileX           = (x + xa + Tile::TILE_SIZE * 0.6f) / Tile::TILE_SIZE;
	tileY           = (y + ya + Tile::TILE_SIZE * 0.2f) / Tile::TILE_SIZE;
	bool upperBound = m_Level->getTile(tileX, tileY)->isSolid();

	return lowerBound || upperBound;
}