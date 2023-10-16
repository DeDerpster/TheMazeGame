#pragma once

#include "Event.h"
#include "Level.h"
#include "Log.h"
#include "Utils.h"

class Entity
{
  protected:
	float        x, y;
	uint16_t     m_SpriteID;
	Level *      m_Level;
	CollisionBox m_CollisionBox;

  public:
	Entity(): x(0.0f), y(0.0f), m_Level(nullptr), m_CollisionBox({{0.0f, 0.0f}, {0.0f, 0.0f}}), m_SpriteID(0) {}
	Entity(float x, float y): x(x), y(y), m_Level(nullptr), m_CollisionBox({{0.0f, 0.0f}, {0.0f, 0.0f}}), m_SpriteID(0) {}
	Entity(float x, float y, Level *level): x(x), y(y), m_Level(level), m_CollisionBox({{0.0f, 0.0f}, {0.0f, 0.0f}}), m_SpriteID(0) {}
	Entity(float x, float y, CollisionBox box, Level *level, uint16_t spriteID): x(x), y(y), m_Level(level), m_CollisionBox(box), m_SpriteID(spriteID) {}
	virtual ~Entity() {}

	virtual void update()                             = 0;
	virtual void render()                             = 0;
	virtual bool eventCallback(const Application::Event &e) = 0;

	float getX() const { return x; }
	float getY() const { return y; }
	virtual bool getIsMoving() { return false; }

	bool doesIntersectWith(Vec2f pos)
	{
		float lowerX = x + m_CollisionBox.lowerBound.x;
		float lowerY = y + m_CollisionBox.lowerBound.y;
		float upperX = x + m_CollisionBox.upperBound.x;
		float upperY = y + m_CollisionBox.upperBound.y;

		return pos.x > lowerX && pos.y > lowerY && pos.x < upperX && pos.y < upperY;
	}
	virtual bool deleteMe() { return false; }

	virtual void changeX(float changeBy) { x += changeBy; }
	virtual void changeY(float changeBy) { y += changeBy; }
	void  setLevel(Level *level) { m_Level = level; }

	bool hasCollidedWith(float xs, float ys, CollisionBox box)
	{
		if(xs + box.lowerBound.x >= x + m_CollisionBox.upperBound.x || x + m_CollisionBox.lowerBound.x >= xs + box.upperBound.x)
			return false;
		if(ys + box.upperBound.y <= y + m_CollisionBox.lowerBound.y || y + m_CollisionBox.upperBound.y <= ys + box.lowerBound.y)
			return false;
		return true;
	}

#ifdef DEBUG
	virtual void imGuiRender()
	{
	}
#endif
};