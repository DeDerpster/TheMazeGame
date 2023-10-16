#pragma once

#include <functional>

#include "../MovableEntity.h"

class Projectile : public MovableEntity
{
  public:
	// The different types of projectiles
	enum class Type : uint8_t
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
	Vec2f   m_StartPos;        // Stores its start position
	float   m_MaxDistance;     // Max distance it can travel
	float   m_Damage;          // Damage it does to a mob
	double  m_Rotation;        // Stores its rotation
	double  m_RotationSpeed;   // Stores its rotation speed
	Entity *spawner;           // The entity which fired the projectile
	bool    hasCollided;       // Stores whether it has collided

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