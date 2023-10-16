#pragma once

#include "../Weapon.h"

class FireStaff : public Weapon
{
  public:
	FireStaff();
	virtual ~FireStaff() override;

	virtual void attack(Level *level, Entity &e, Direction dir, bool hold) override;
};