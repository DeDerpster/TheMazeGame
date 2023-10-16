#pragma once

#include "../Entity.h"

#include "Direction.h"

class MovableEntity : public Entity
{
  protected:
	float     m_Speed;    // Speed of the movable entity
	bool      isMoving;   // Stores whether it is moving or not
	Direction m_Dir;      // Stores the direction it is going in

	bool isGhost;   // Variable only used with debugging but allows an entity to ignore collision detection

  public:
	MovableEntity();
	MovableEntity(float x, float y, float size, Level *level, Sprite::ID spriteID);
	MovableEntity(float x, float y, float size, CollisionBox box, Level *level, Sprite::ID spriteID);
	MovableEntity(float x, float y, float size, float speed, Direction dir, CollisionBox box, Level *level, Sprite::ID spriteID);
	virtual ~MovableEntity() override;

	virtual bool eventCallback(const Event::Event &e) override;

	virtual void move(float xa, float ya);
	virtual void move(Vec2f ratio);   // Allows movement with a ratio to maximise speed (and have it accurate)

	// Functions to check if it can move in a given direction
	bool canMove(float xa, float ya);
	bool canMove(Vec2f ratio);

	virtual bool      getIsMoving() override { return isMoving; }
	virtual Direction getDirection() { return m_Dir; }
	CollisionBox      getMovingCollisionBox();
};