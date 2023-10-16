#pragma once

#include "Event.h"
#include "Level.h"
#include "Log.h"

class Entity
{
  protected:
	float  x, y;
	bool   isInControl = false;
	Level *m_Level;

  public:
	Entity(): x(0.0f), y(0.0f), m_Level(nullptr) {}
	Entity(float x, float y): x(x), y(y), m_Level(nullptr) {}
	Entity(float x, float y, Level *level): x(x), y(y), m_Level(level) {}
	virtual ~Entity() {}

	virtual void update()                             = 0;
	virtual void render()                             = 0;
	virtual void eventCallback(Application::Event &e) = 0;

	float getX() const { return x; }
	float getY() const { return y; }
	void  setIsInControl(bool i_isInControl) { isInControl = i_isInControl; }
	void  changeX(float changeBy) { x += changeBy; }
	void  changeY(float changeBy) { y += changeBy; }
	void  setLevel(Level *level) { m_Level = level; }

	virtual bool getIsMoving() { return false; }
#ifdef DEBUG
	virtual void imGuiRender() = 0;
#endif
};