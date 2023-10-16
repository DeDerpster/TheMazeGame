#pragma once

#include "Mob.h"

#include <string>

class NPC : public Mob
{
  public:
	enum class Type
	{
		Follower,
		Enemy
	};

	enum class Race
	{
		Frost,
		Fire,
		Dark
	};

  private:
    std::string m_Name;

    enum class AttackMove
    {
	  None,
	  Dodge,
	  RunAway,
	  RunAwayAlongX,
	  RunAwayAlongY,
	  GoToPoint,
	  Attack
	};
	AttackMove m_Attack;

	Vec2f    m_Center;
	Vec2f    m_NextPos;
	bool     m_NextPosActive;
	uint32_t m_TimeSinceMoved;
	uint32_t m_WaitFor;

	bool isRunningAway;

	void generateInventory(Race race);

	void findPath(Vec2f dest, float speed);
	void attack();
	void follow();
	void roam();

	void generateNextPos();

  public:
	NPC();
	NPC(float x, float y, Level *level, Type type);
	NPC(float x, float y, Level *level, Type type, Race race);
	virtual ~NPC() override;

	virtual void update() override;
	virtual void render() override;
#ifdef DEBUG
	virtual void imGuiRender() override;
#endif

	void goToPointInRoom();

	virtual void setFollowing(Mob *following) override;

	virtual bool eventCallback(const Event::Event &e) override;
};