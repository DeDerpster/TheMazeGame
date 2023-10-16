#pragma once

#include "Mob.h"

#include <string>
#include <vector>

#include "AnimatedSprite.h"
#include "Utils.h"

class Follower : public Mob
{
  private:
	std::string                             m_Name;
	std::unique_ptr<Sprite::AnimatedSprite> m_NorthAnimation;
	std::unique_ptr<Sprite::AnimatedSprite> m_SouthAnimation;
	std::unique_ptr<Sprite::AnimatedSprite> m_EastAnimation;
	std::unique_ptr<Sprite::AnimatedSprite> m_WestAnimation;

	// std::vector<Vec2f> m_Path;
	Entity *following = nullptr;

	void setupAnimations();

  public:
	Follower();
	Follower(float x, float y);
	Follower(float x, float y, Level *level);
	virtual ~Follower() override;

	virtual void update() override;
	virtual void render() override;
#ifdef DEBUG
	virtual void imGuiRender() override;
#endif

	void setFollower(Entity *e)
	{
		following = e;
	}

	virtual void eventCallback(Application::Event &e) override;
};