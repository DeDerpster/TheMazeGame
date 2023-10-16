#pragma once

#include "../Weapon.h"

class Crossbow : public Weapon
{
  public:
	Crossbow();
	virtual ~Crossbow() override;

	virtual void attack(Level *level, Entity &e, Direction dir, bool hold) override;
};