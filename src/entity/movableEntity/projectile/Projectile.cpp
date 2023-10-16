#include "Projectile.h"

#include "Entity.h"
#include "KeyDefinitions.h"
#include "Level.h"
#include "Sprite.h"

#define defaultBox           \
	{                        \
		{25, 20}, { 60, 20 } \
	}

Projectile::Projectile(float startX, float startY, float size, float damage, Direction dir, Entity *spawner, Level *level)
	: MovableEntity(startX, startY, size, 7.0f, dir, defaultBox, level, PROJECTILE_FIRE), m_StartPos({startX, startY}), m_MaxDistance(10 * TILE_SIZE), m_Damage(damage), spawner(spawner), hasCollided(false)
{
}

Projectile::Projectile(float startX, float startY, float size, float damage, float speed, Direction dir, Entity *spawner, Level *level, CollisionBox box)
	: MovableEntity(startX, startY, size, speed, dir, box, level, PROJECTILE_FIRE), m_StartPos({startX, startY}), m_MaxDistance(10 * TILE_SIZE), m_Damage(damage), spawner(spawner), hasCollided(false)
{
}

Projectile::Projectile(float startX, float startY, float size, float maxDistance, float damage, float speed, Direction dir, Entity *spawner, Level *level, CollisionBox box, std::function<void(float, float, Direction, Level *)> collisionFunc)
	: MovableEntity(startX, startY, size, speed, dir, box, level, PROJECTILE_FIRE), m_StartPos({startX, startY}), m_MaxDistance(maxDistance), m_Damage(damage), spawner(spawner), hasCollided(false), m_CollisionFunction(collisionFunc)
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
		Entity *colE = m_Level->entityCollisionDetection(x + xs, y + ys, m_CollisionBox);
		if(!isGhost && (m_Level->collisionDetection(x + xs, y + ys, m_CollisionBox) || (colE != nullptr && colE != spawner)))
		{
			// TODO: Check if this is valid (or if the enemy has been killed)
			Mob *mSpawner = dynamic_cast<Mob *>(spawner);
			if(colE)
			{
				Mob *mob = dynamic_cast<Mob *>(colE);
				if(mob)
					mob->dealDamage(m_Damage);
				if(mSpawner)
					mSpawner->hasHitTarget(m_Damage);
			}
			else if(mSpawner)
				mSpawner->hasMissedTarget();
			m_CollisionFunction(x, y, m_Dir, m_Level);
			hasCollided = true;
		}
		else
		{
			isMoving = true;
			x += xs;
			y += ys;
		}
	}
}

bool Projectile::eventCallback(const Event::Event &e)
{
	return MovableEntity::eventCallback(e);
}

void Projectile::render()
{
	Render::sprite(x, y, directionToRotation(m_Dir), width, height, m_SpriteID);
}

void Projectile::changeX(float changeBy)
{
	x += changeBy;
	m_StartPos.x += changeBy;
}
void Projectile::changeY(float changeBy)
{
	y += changeBy;
	m_StartPos.y += changeBy;
}