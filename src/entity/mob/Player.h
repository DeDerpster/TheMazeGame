#pragma once

#include "AnimatedSprite.h"
#include "Mob.h"

class Player : public Mob
{
  private:
	std::unique_ptr<Sprite::AnimatedSprite> m_NorthAnimation;
	std::unique_ptr<Sprite::AnimatedSprite> m_SouthAnimation;
	std::unique_ptr<Sprite::AnimatedSprite> m_EastAnimation;
	std::unique_ptr<Sprite::AnimatedSprite> m_WestAnimation;

	void setupAnimations();

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

	virtual void eventCallback(Application::Event &e) override;
};