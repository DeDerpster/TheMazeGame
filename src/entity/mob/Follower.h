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
	std::unique_ptr<Render::AnimatedSprite> m_NorthAnimation;
	std::unique_ptr<Render::AnimatedSprite> m_SouthAnimation;
	std::unique_ptr<Render::AnimatedSprite> m_EastAnimation;
	std::unique_ptr<Render::AnimatedSprite> m_WestAnimation;

	// std::vector<Vec2f> m_Path;
	Entity *following = nullptr;

	bool findingPath = false;

	void setupAnimations();

	void findPath();

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

	virtual bool eventCallback(const Application::Event &e) override;
};