#pragma once

#include "../Weapon.h"

class Bow : public Weapon
{
  public:
	Bow();
	virtual ~Bow() override;

	virtual void attack(Level *level, Entity &e, Direction dir, bool hold) override;
};