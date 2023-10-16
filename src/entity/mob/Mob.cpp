#include "Mob.h"
#include "Tile.h"

Mob::Mob()
	: Entity(), m_Speed(7.0f), isMoving(false), m_CollisionBox({{(float) Tile::TILE_SIZE * 0.37f, (float) -Tile::TILE_SIZE * 0.04f}, {(float) Tile::TILE_SIZE * 0.6f, (float) Tile::TILE_SIZE * 0.2f}}), m_Dir(Direction::SOUTH)
{
}

Mob::Mob(float x, float y)
	: Entity(x, y), m_Speed(7.0f), isMoving(false), m_CollisionBox({{(float) Tile::TILE_SIZE * 0.37f, (float) -Tile::TILE_SIZE * 0.04f}, {(float) Tile::TILE_SIZE * 0.6f, (float) Tile::TILE_SIZE * 0.2f}}), m_Dir(Direction::SOUTH)
{
}

Mob::Mob(float x, float y, Level *level)
	: Entity(x, y, level), m_Speed(7.0f), isMoving(false), m_CollisionBox({{(float) Tile::TILE_SIZE * 0.37f, (float) -Tile::TILE_SIZE * 0.04f}, {(float) Tile::TILE_SIZE * 0.6f, (float) Tile::TILE_SIZE * 0.2f}}), m_Dir(Direction::SOUTH)
{
}

Mob::~Mob()
{
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
	if(xa < 0)
		m_Dir = Direction::WEST;
	else if(xa > 0)
		m_Dir = Direction::EAST;
	if(ya < 0)
		m_Dir = Direction::SOUTH;
	else if(ya > 0)
		m_Dir = Direction::NORTH;
}
