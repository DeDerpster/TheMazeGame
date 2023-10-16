#pragma once

#include "Mob.h"
#include "MovableEntity.h"

class Projectile : public MovableEntity
{
  protected:
	Vec2f   m_StartPos;
	float   m_MaxDistance;
	float   m_Damage;
	float   m_Size;
	Mob *   spawner;
	bool    hasCollided;

  public:
	Projectile(float startX, float startY, float damage, Direction dir, Mob *spawner, Level *level);
	Projectile(float startX, float startY, float damage, float speed, Direction dir, Mob *spawner, Level *level, CollisionBox box);
	Projectile(float startX, float startY, float maxDistance, float damage, float speed, Direction dir, Mob *spawner, Level *level, CollisionBox box);
	virtual ~Projectile() {}

	virtual void update() override;
	virtual void render() override;
	virtual bool eventCallback(const Application::Event &e) override { return false; }

	virtual void changeX(float changeBy) override;
	virtual void changeY(float changeBy) override;
	virtual bool deleteMe() override { return hasCollided; }
};