#pragma once

#include "Mob.h"

#include <string>
#include <vector>

#include "Utils.h"

class Follower : public Mob
{
  private:
	std::string m_Name;
	Entity *    following = nullptr;

	bool findingPath = false;

	void findPath();

  public:
	Follower();
	Follower(float x, float y);
	Follower(float x, float y, Level *level);
	Follower(float x, float y, Level *level, uint16_t spriteID);
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

	virtual bool eventCallback(const Event::Event &e) override;
};