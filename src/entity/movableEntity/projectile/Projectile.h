#pragma once

#include "MovableEntity.h"

class Projectile : public MovableEntity
{
  protected:
	Vec2f   m_StartPos;
	float   m_MaxDistance;
	float   m_Damage;
	Entity *spawner;
	bool    hasCollided;

  public:
	Projectile(float startX, float startY, float maxDistance, float damage, float speed, Direction dir, Entity *spawner, Level *level);
	virtual ~Projectile() {}

	virtual void update() override;
	virtual void render() override;
	virtual bool eventCallback(const Application::Event &e) override { return false; }

	virtual bool deleteMe() override { return hasCollided; }
};