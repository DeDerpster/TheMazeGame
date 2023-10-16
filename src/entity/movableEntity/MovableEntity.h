#pragma once

#include "Entity.h"

class MovableEntity : public Entity
{
  protected:
	float     m_Speed;
	bool      isMoving;
	Direction m_Dir;

	bool isGhost;

  public:
	MovableEntity();
	MovableEntity(float x, float y, float size, Level *level, Sprite::ID spriteID);
	MovableEntity(float x, float y, float size, CollisionBox box, Level *level, Sprite::ID spriteID);
	MovableEntity(float x, float y, float size, float speed, Direction dir, CollisionBox box, Level *level, Sprite::ID spriteID);
	virtual ~MovableEntity() override;

	virtual bool eventCallback(const Event::Event &e) override;

	virtual void move(float xa, float ya);
	virtual void move(Vec2f ratio);

	bool canMove(float xa, float ya);
	bool canMove(Vec2f ratio);

	virtual bool      getIsMoving() override { return isMoving; }
	virtual Direction getDirection() { return m_Dir; }
	CollisionBox      getMovingCollisionBox();
};