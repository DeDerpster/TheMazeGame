#include "Projectile.h"

#include "Sprite.h"

#define defaultBox           \
	{                        \
		{25, 20}, { 60, 20 } \
	}

Projectile::Projectile(float startX, float startY, float maxDistance, float damage, float speed, Direction dir, Entity *spawner, Level *level)
	: MovableEntity(startX, startY, speed, dir, defaultBox, level, PROJECTILE_FIRE), m_StartPos({startX, startY}), m_MaxDistance(maxDistance), m_Damage(damage), spawner(spawner), hasCollided(false)
{
}

void Projectile::update()
{
	if(!hasCollided)
	{
		if(distBetweenVec2f({x, y}, m_StartPos) > m_MaxDistance)
			hasCollided = true;
		float xs = 0;
		float ys = 0;
		if(m_Dir == Direction::north)
			ys = m_Speed;
		else if(m_Dir == Direction::south)
			ys = -m_Speed;
		else if(m_Dir == Direction::east)
			xs = m_Speed;
		else
			xs = -m_Speed;
		if(!isGhost && m_Level->collisionDetection(x + xs, y + ys, m_CollisionBox))
			hasCollided = true;
		else
		{
			isMoving = true;
			x += xs;
			y += ys;
		}
	}
}

void Projectile::render()
{
	Render::Sprite::getSprite(m_SpriteID)->render(x, y, directionToRotation(m_Dir), 50);
}