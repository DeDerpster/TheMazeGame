#include "MovableEntity.h"

MovableEntity::MovableEntity()
	: Entity(), m_Speed(7.0f), isMoving(false), m_Dir(Direction::south), isGhost(false) {}

MovableEntity::MovableEntity(float x, float y, Level *level)
	: Entity(x, y, level), m_Speed(7.0f), isMoving(false), m_Dir(Direction::south), isGhost(false) {}

MovableEntity::MovableEntity(float x, float y, CollisionBox box, Level *level, uint16_t spriteID)
	: Entity(x, y, box, level, spriteID), m_Speed(7.0f), isMoving(false), m_Dir(Direction::south), isGhost(false) {}

MovableEntity::MovableEntity(float x, float y, float speed, Direction dir, CollisionBox box, Level *level, uint16_t spriteID)
	: Entity(x, y, box, level, spriteID), m_Speed(speed), isMoving(false), m_Dir(dir), isGhost(false) {}

MovableEntity::~MovableEntity() {}

void MovableEntity::move(float xa, float ya)
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
			m_Dir = Direction::south;
		else if(ya > 0)
			m_Dir = Direction::north;
		if(xa < 0)
			m_Dir = Direction::west;
		else if(xa > 0)
			m_Dir = Direction::east;
	}
	else
	{
		if(xa < 0)
			m_Dir = Direction::west;
		else if(xa > 0)
			m_Dir = Direction::east;
		if(ya < 0)
			m_Dir = Direction::south;
		else if(ya > 0)
			m_Dir = Direction::north;
	}
}

void MovableEntity::move(Vec2f ratio)
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