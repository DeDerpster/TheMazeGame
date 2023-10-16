#pragma once

#include "../Weapon.h"

class DarkStaff : public Weapon
{
  public:
	DarkStaff();
	virtual ~DarkStaff() override;

	virtual void attack(Level *level, Entity &e, Direction dir, bool hold) override;
};