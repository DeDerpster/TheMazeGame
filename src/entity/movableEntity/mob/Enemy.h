#pragma once

#include "Mob.h"

class Enemy : public Mob
{
  protected:
	bool isRunningAway;

	void generateInventory();

  public:
	Enemy();
	Enemy(float x, float y);
	Enemy(float x, float y, Level *level);
	Enemy(float x, float y, Level *level, uint16_t spriteID);
	virtual ~Enemy();

	virtual void update() override;
	virtual void render() override;
	virtual bool eventCallback(const Event::Event &e) override;

#ifdef DEBUG
	virtual void imGuiRender() override;
#endif
};