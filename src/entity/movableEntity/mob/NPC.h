#pragma once

#include "Mob.h"

#include <string>

class NPC : public Mob
{
  public:
	// Stores the different types of NPCs (only used for creation)
	enum class Type : uint8_t
	{
		Follower,
		Enemy
	};

	// Stores the race of NPC (only used for creation)
	enum class Race : uint8_t
	{
		Frost,
		Fire,
		Dark
	};

  private:
	std::string m_Name;   // Stores the name of the NPC

	// Different attack moves an NPC can make
	enum class AttackMove : uint8_t
	{
	  None,
	  Dodge,
	  RunAway,
	  RunAwayAlongX,
	  RunAwayAlongY,
	  GoToPoint,
	  Attack
	};
	AttackMove m_Attack;   // Stores the current attack the enemy is doing

	// Stores information for attacking
	Vec2f    m_Center;           // Stores the coordinates of the room current room
	Vec2f    m_NextPos;          // Stores the next position of the NPC
	bool     m_NextPosActive;    // Stores whether the next position is currently being used
	uint32_t m_TimeSinceMoved;   // Stores the time since the NPC has changed attack moves
	uint32_t m_WaitFor;          // Stores how long it should wait for before it should change attack move

	// Generates the inventory of the follower
	void generateInventory(Race race);

	void findPath(Vec2f dest, float speed);   // Finds shortest path to point
	void attack();                            // Function that controls the attacking aspect of NPC
	void follow();                            // Function that controls the following aspect of NPC
	void roam();                              // Function that controls the roaming aspect of NPC

	void generateNextPos();   // Generates a random next postion

	void goToPointInRoom();   // Goes to point random point in room when attacking

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

	virtual void setFollowing(Mob *following) override;

	virtual bool eventCallback(const Event::Event &e) override;

	virtual void changeX(float changeBy) override;
	virtual void changeY(float changeBy) override;
};