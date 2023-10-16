#include "Projectile.h"

#define defaultBox                                           \
	{                                                        \
		{-width / 2, -height / 2}. { width / 2, height / 2 } \
	}

Projectile::Projectile(float startX, float startY, float maxDistance, float damage, float speed, Direction dir, float width, float height, Entity *spawner, Level *level)
	: MovableEntity(startX, startY, speed, dir, defaultBox, level, 0), m_StartPos({startX, startY}), m_MaxDistance(maxDistance), m_Damage(damage), spawner(spawner), hasCollided(false) {}
