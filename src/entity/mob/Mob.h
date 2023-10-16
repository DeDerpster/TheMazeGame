#pragma once

#include "Entity.h"
#include "Utils.h"

class Mob : public Entity
{
  protected:
	float        m_Speed;
	bool         isMoving;
	bool         isGhost = false;
	CollisionBox m_CollisionBox;
	Direction    m_Dir;

  public:
	Mob();
	Mob(float x, float y);
	Mob(float x, float y, Level *level);
	virtual ~Mob();

	void move(float xa, float ya);
	void move(Vec2f ratio);

	virtual void render()                             = 0;
	virtual void update()                             = 0;
	virtual void eventCallback(Application::Event &e) = 0;

	virtual bool      getIsMoving() override { return isMoving; }
	virtual Direction getDirection() { return m_Dir; }
#ifdef DEBUG
	virtual void imGuiRender() = 0;
#endif
};