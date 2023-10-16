#pragma once

#include "AnimatedSprite.h"
#include "Mob.h"

class Player : public Mob
{
  public:
	Player();
	Player(float x, float y);
	Player(float x, float y, Level *level);
	virtual ~Player() override;

	virtual void update() override;
	virtual void render() override;
#ifdef DEBUG
	virtual void imGuiRender() override;
#endif

	virtual bool eventCallback(const Event::Event &e) override;

	virtual void setFollowing(Mob *following) override;
	virtual void setEnemy(Mob *enemy) override;
};