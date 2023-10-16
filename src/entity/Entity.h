#pragma once

#include "Event.h"
#include "Log.h"
#include "Sprite.h"
#include "Utils.h"

class Level;
class Entity
{
  protected:
	float        x, y;
	float        width, height;
	Sprite::ID   m_SpriteID;
	Level *      m_Level;
	CollisionBox m_CollisionBox;

  public:
	Entity();
	Entity(float x, float y, float size, Sprite::ID spriteID);
	Entity(float x, float y, float size, Level *level, Sprite::ID spriteID);
	Entity(float x, float y, float size, CollisionBox box, Level *level, Sprite::ID spriteID);
	virtual ~Entity();

	virtual void update()                             = 0;
	virtual void render()                             = 0;
	virtual bool eventCallback(const Event::Event &e);

	float        getX() const;
	float        getY() const;
	float         getWidth() const;
	float         getHeight() const;
	virtual bool getIsMoving();
	CollisionBox &getCollisionBox() { return m_CollisionBox; }
	bool doesIntersectWith(Vec2f pos);
	virtual bool  hasCollidedWith(float xs, float ys, CollisionBox box);
	virtual bool deleteMe();

	virtual void changeX(float changeBy);
	virtual void changeY(float changeBy);
	void         setLevel(Level *level);


#ifdef DEBUG
	virtual void imGuiRender();
#endif
};