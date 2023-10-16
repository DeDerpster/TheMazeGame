#pragma once

#include "Event.h"
#include "Level.h"
#include "Log.h"
#include "Utils.h"

class Entity
{
  protected:
	float        x, y;
	float        width, height;
	uint16_t     m_SpriteID;
	Level *      m_Level;
	CollisionBox m_CollisionBox;

  public:
	Entity();
	Entity(float x, float y, float size, uint16_t spriteID);
	Entity(float x, float y, float size, Level *level, uint16_t spriteID);
	Entity(float x, float y, float size, CollisionBox box, Level *level, uint16_t spriteID);
	virtual ~Entity();

	virtual void update()                             = 0;
	virtual void render()                             = 0;
	virtual bool eventCallback(const Event::Event &e);

	float        getX() const;
	float        getY() const;
	virtual bool getIsMoving();

	bool doesIntersectWith(Vec2f pos);

	virtual bool deleteMe();

	virtual void changeX(float changeBy);
	virtual void changeY(float changeBy);
	void         setLevel(Level *level);
	CollisionBox &getCollisionBox() { return m_CollisionBox; }

	bool hasCollidedWith(float xs, float ys, CollisionBox box);

#ifdef DEBUG
	virtual void imGuiRender();
#endif
};