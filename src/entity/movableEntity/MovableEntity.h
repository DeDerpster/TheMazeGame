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
	MovableEntity(float x, float y, Level *level);
	MovableEntity(float x, float y, CollisionBox box, Level *level, uint16_t spriteID);
	MovableEntity(float x, float y, float speed, Direction dir, CollisionBox box, Level *level, uint16_t spriteID);
	virtual ~MovableEntity();

	virtual void move(float xa, float ya);
	virtual void move(Vec2f ratio);

	virtual bool      getIsMoving() override { return isMoving; }
	virtual Direction getDirection() { return m_Dir; }
};