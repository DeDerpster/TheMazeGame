#pragma once

#include "MovableEntity.h"

class Projectile : public MovableEntity
{
  protected:
	Vec2f   m_StartPos;
	float   m_MaxDistance;
	float   m_Damage;
	float   m_Size;
	Entity *spawner;
	bool    hasCollided;

  public:
	Projectile(float startX, float startY, float damage, Direction dir, Entity *spawner, Level *level);
	Projectile(float startX, float startY, float maxDistance, float damage, float speed, Direction dir, Entity *spawner, Level *level, CollisionBox box);
	virtual ~Projectile() {}

	virtual void update() override;
	virtual void render() override;
	virtual bool eventCallback(const Application::Event &e) override { return false; }

	virtual bool deleteMe() override { return hasCollided; }
};