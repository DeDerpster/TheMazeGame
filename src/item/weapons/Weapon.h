#pragma once

#include "Item.h"

#include "Entity.h"
#include "Event.h"

class Level;

class Weapon : public Item
{
  protected:
	float m_Damage;
	uint16_t m_Cooldown;
	uint16_t m_CooldownMax;

  public:
	Weapon(float damage);
	Weapon(const char *name, float damage, Sprite::ID spriteID);
	Weapon(const char *name, float damage, uint16_t cooldown, Sprite::ID spriteID);
	virtual ~Weapon() override;

	virtual void update();

	virtual void attack(Level *level, Entity &e, Direction dir, bool hold) = 0;

	bool canUse() { return m_Cooldown == 0; }
};