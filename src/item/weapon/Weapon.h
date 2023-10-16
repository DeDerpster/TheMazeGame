#pragma once

#include "Item.h"

#include "Event.h"
#include "Level.h"

class Weapon : public Item
{
  protected:
	float m_Damage;

  public:
	Weapon(float damage);
	Weapon(const char *name, float damage, uint16_t spriteID);
	virtual ~Weapon();

	virtual void attack(Level *level, Entity &e, Direction dir) = 0;
};