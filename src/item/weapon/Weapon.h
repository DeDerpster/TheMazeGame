#pragma once

#include "../Item.h"

#include "entity/Entity.h"

#include "Direction.h"

class Weapon : public Item
{
  protected:
	float    m_Damage;     // Stores the base damage of the weapon
	uint16_t m_Cooldown;   // Stores the cooldown and max cooldown
	uint16_t m_CooldownMax;

  public:
	Weapon(float damage);
	Weapon(const char *name, float damage, Sprite::ID spriteID);
	Weapon(const char *name, float damage, uint16_t cooldown, Sprite::ID spriteID);
	virtual ~Weapon() override;

	virtual void update();

	// Function for an entity attacking on a level
	virtual void attack(Level *level, Entity &e, Direction dir, bool hold) = 0;

	bool canUse() { return m_Cooldown == 0; }
};