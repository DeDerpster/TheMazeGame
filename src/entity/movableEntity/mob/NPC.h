#pragma once

#include "Mob.h"

#include <string>

class NPC : public Mob
{
  private:
	std::string m_Name;
	Entity *    following;
	Entity *    attacking;

	bool findingPath;
	bool isRunningAway;

	void generateInventory();
	void findPath();

	void attack();
	void follow();

  public:
	NPC();
	NPC(float x, float y);
	NPC(float x, float y, Level *level);
	NPC(float x, float y, Level *level, uint16_t spriteID);
	virtual ~NPC() override;

	virtual void update() override;
	virtual void render() override;
#ifdef DEBUG
	virtual void imGuiRender() override;
#endif

	const Entity *getFollowing() const
	{
		return following;
	}

	const Entity *getAttacking() const
	{
		return attacking;
	}

	void setFollower(Entity *e)
	{
		following = e;
	}
	void setAttacking(Entity *e)
	{
		attacking = e;
	}

	virtual bool eventCallback(const Event::Event &e) override;
};