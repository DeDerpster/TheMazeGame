#pragma once

#include "Item.h"

#include "Event.h"
#include "Level.h"
#include "Mob.h"

class Weapon : public Item
{
  protected:
	float m_Damage;
	uint16_t m_Cooldown;
	uint16_t m_CooldownMax;

  public:
	Weapon(float damage);
	Weapon(const char *name, float damage, uint16_t spriteID);
	Weapon(const char *name, float damage, uint16_t cooldown, uint16_t spriteID);
	virtual ~Weapon();

	virtual void update();

	virtual void attack(Level *level, Mob &e, Direction dir, bool hold) = 0;
};