#pragma once

#include <functional>

#include "MovableEntity.h"

class Projectile : public MovableEntity
{
  public:
	enum class Type
	{
		Arrow,
		Boomerang,
		Dark,
		Fire,
		Frost,
		Gold,
		Nature,
		Rock
	};

  protected:
	Vec2f   m_StartPos;
	float   m_MaxDistance;
	float   m_Damage;
	Entity *spawner;
	bool    hasCollided;
	double  m_Rotation;
	double  m_RotationSpeed;

	std::function<void(float, float, Direction, Level *)> m_CollisionFunction;

  public:
	Projectile(float startX, float startY, float damage, Direction dir, Entity *spawner, Level *level, Type type);
	Projectile(float startX, float startY, float size, float damage, Direction dir, Entity *spawner, Level *level);
	Projectile(float startX, float startY, float size, float damage, float speed, Direction dir, Entity *spawner, Level *level, CollisionBox box);
	Projectile(float startX, float startY, float size, float maxDistance, float damage, float speed, Direction dir, Entity *spawner, Level *level, CollisionBox box, std::function<void(float, float, Direction, Level *)> collisionFunc);
	virtual ~Projectile() override {}

	virtual void update() override;
	virtual void render() override;
	virtual bool eventCallback(const Event::Event &e) override;

	virtual void changeX(float changeBy) override;
	virtual void changeY(float changeBy) override;
	virtual bool deleteMe() override { return hasCollided; }

	CollisionBox getEntityBox() { return {{20, 20}, m_CollisionBox.upperBound}; }
};