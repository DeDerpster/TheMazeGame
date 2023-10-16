#pragma once

#include "2dVec.h"
#include "Log.h"
#include "event/Event.h"
#include "rendering/sprite/Sprite.h"

#include "CollisionBox.h"

class Level;
class Entity
{
  protected:
	float        x, y;             // Stores the position of the entity
	float        width, height;    // Stores its visual width and height
	Sprite::ID   m_SpriteID;       // Stores its sprite ID
	Level *      m_Level;          // Stores the level it is at
	CollisionBox m_CollisionBox;   // Stores its collision box NOTE: Slightly different from its visual width and height

  public:
	Entity();
	Entity(float x, float y, float size, Sprite::ID spriteID);
	Entity(float x, float y, float size, Level *level, Sprite::ID spriteID);
	Entity(float x, float y, float size, CollisionBox box, Level *level, Sprite::ID spriteID);
	virtual ~Entity();

	virtual void update() = 0;
	virtual void render() = 0;
	virtual bool eventCallback(const Event::Event &e);

	float         getX() const;
	float         getY() const;
	float         getWidth() const;
	float         getHeight() const;
	virtual bool  getIsMoving();
	CollisionBox &getCollisionBox() { return m_CollisionBox; }

	bool          doesIntersectWith(Vec2f pos);
	virtual bool  hasCollidedWith(float xs, float ys, CollisionBox box);
	// Function that returns whether it should be deleted
	virtual bool deleteMe();

	virtual void changeX(float changeBy);
	virtual void changeY(float changeBy);
	void         setLevel(Level *level);

#ifdef DEBUG
	virtual void imGuiRender();
#endif
};