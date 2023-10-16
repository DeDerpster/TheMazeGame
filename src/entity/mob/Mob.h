#pragma once

#include "Entity.h"

#include <vector>

#include "Item.h"
#include "Utils.h"

class Mob : public Entity
{
  protected:
	float        m_Speed;
	bool         isMoving;
	Direction    m_Dir;

	bool isGhost     = false;
	bool isInControl = false;

	std::vector<Item *> inventory;

  public:
	Mob();
	Mob(float x, float y);
	Mob(float x, float y, Level *level);
	virtual ~Mob();

	void move(float xa, float ya);
	void move(Vec2f ratio);

	void pickUp(Item *item);

	virtual void render()                             = 0;
	virtual void update()                             = 0;
	virtual bool eventCallback(const Application::Event &e) = 0;

	virtual bool      getIsMoving() override { return isMoving; }
	virtual Direction getDirection() { return m_Dir; }
	void              setIsInControl(bool i_isInControl) { isInControl = i_isInControl; }
#ifdef DEBUG
	virtual void imGuiRender() = 0;
#endif
};