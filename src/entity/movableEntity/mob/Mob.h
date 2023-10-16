#pragma once

#include "MovableEntity.h"

#include <vector>

#include "AnimatedSprite.h"
#include "Item.h"
#include "Utils.h"

class Mob : public MovableEntity
{
  protected:
	std::vector<Item *> inventory;

	std::unique_ptr<Render::AnimatedSprite> m_NorthAnimation;
	std::unique_ptr<Render::AnimatedSprite> m_SouthAnimation;
	std::unique_ptr<Render::AnimatedSprite> m_EastAnimation;
	std::unique_ptr<Render::AnimatedSprite> m_WestAnimation;

	bool isInControl = false;

  public:
	Mob();
	Mob(float x, float y);
	Mob(float x, float y, Level *level);
	Mob(float x, float y, Level *level, uint16_t spriteID);
	virtual ~Mob();

	void pickUp(Item *item);
	void setupAnimations();

	virtual void render();
	virtual void update();
	virtual bool eventCallback(const Application::Event &e) = 0;

	void setIsInControl(bool i_isInControl) { isInControl = i_isInControl; }
#ifdef DEBUG
	virtual void imGuiRender() = 0;
#endif
};