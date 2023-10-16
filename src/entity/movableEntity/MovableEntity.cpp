#include "MovableEntity.h"

#include "layer/level/Level.h"

MovableEntity::MovableEntity()
	: Entity(), m_Speed(7.0f), isMoving(false), m_Dir(Direction::south), isGhost(false) {}

MovableEntity::MovableEntity(float x, float y, float size, Level *level, Sprite::ID spriteID)
	: Entity(x, y, size, level, spriteID), m_Speed(7.0f), isMoving(false), m_Dir(Direction::south), isGhost(false) {}

MovableEntity::MovableEntity(float x, float y, float size, CollisionBox box, Level *level, Sprite::ID spriteID)
	: Entity(x, y, size, box, level, spriteID), m_Speed(7.0f), isMoving(false), m_Dir(Direction::south), isGhost(false) {}

MovableEntity::MovableEntity(float x, float y, float size, float speed, Direction dir, CollisionBox box, Level *level, Sprite::ID spriteID)
	: Entity(x, y, size, box, level, spriteID), m_Speed(speed), isMoving(false), m_Dir(dir), isGhost(false) {}

MovableEntity::~MovableEntity() {}

void MovableEntity::move(float xa, float ya)
{
	if(!isGhost)
	{
		// Uses directional collision check to see which direction it can move in
		auto [colX, colY] = m_Level->directionalCollisionCheck(x, y, xa, ya, getMovingCollisionBox());

		// If it cant move in a given direction it sets that offset to 0
		if(colX)
			xa = 0;
		if(colY)
			ya = 0;
	}

	if(xa == 0 && ya == 0)
	{
		isMoving = false;
		return;
	}

	// Moves in the direction
	isMoving = true;
	x += xa;
	y += ya;

	// Works out which direction it is going in (using whichever one direction has the most change)
	if(fabs(xa) > fabs(ya) && xa != 0)
	{
		if(xa < 0)
			m_Dir = Direction::west;
		else if(xa > 0)
			m_Dir = Direction::east;
	}
	else   // NOTE: If xa is 0, ya cannot be zero because of the check earlier
	{
		if(ya < 0)
			m_Dir = Direction::south;
		else if(ya > 0)
			m_Dir = Direction::north;
	}
}

void MovableEntity::move(Vec2f ratio)
{
	if(ratio.y == 0)
		move(ratio.x > 0 ? m_Speed : -m_Speed, 0.0f);
	else if(ratio.x == 0)
		move(0.0f, ratio.y > 0 ? m_Speed : -m_Speed);
	else
	{
		// Calculates the real x and y offsets from the speed using pythagoras' theorem
		float speedSquared = m_Speed * m_Speed;
		float sum          = std::fabs(ratio.x) + std::fabs(ratio.y);
		float sumSquared   = sum * sum;
		float timesBy      = std::sqrt((sumSquared * speedSquared) / (ratio.x * ratio.x + ratio.y * ratio.y));
		move((ratio.x * timesBy) / sum, (ratio.y * timesBy) / sum);
	}
}

bool MovableEntity::canMove(float xa, float ya)
{
	if(isGhost) return true;

	// Uses directional collision check to see which direction it cannot move
	auto [colX, colY] = m_Level->directionalCollisionCheck(x, y, xa, ya, getMovingCollisionBox());

	return !(colX && colY);
}

bool MovableEntity::canMove(Vec2f ratio)
{
	if(isGhost) return true;

	// Calculates the movement in each direction and passes it to canMove(float xs, float ys)
	if(ratio.y == 0)
		return canMove(m_Speed, 0.0f);
	else if(ratio.x == 0)
		return canMove(0.0f, m_Speed);
	else
	{
		float speedSquared = m_Speed * m_Speed;
		float sum          = std::fabs(ratio.x) + std::fabs(ratio.y);
		float sumSquared   = sum * sum;
		float timesBy      = std::sqrt((sumSquared * speedSquared) / (ratio.x * ratio.x + ratio.y * ratio.y));
		return canMove((ratio.x * timesBy) / sum, (ratio.y * timesBy) / sum);
	}
}

bool MovableEntity::eventCallback(const Event::Event &e)
{
	return Entity::eventCallback(e);
}

CollisionBox MovableEntity::getMovingCollisionBox()
{
	// Returns a slightly altered collision box that is used for moving (slightly different as it is a top down game)
	return {m_CollisionBox.lowerBound,
			{m_CollisionBox.upperBound.x, m_CollisionBox.upperBound.y / 4.0f}};
}