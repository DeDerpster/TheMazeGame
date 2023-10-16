#pragma once

#include "../Weapon.h"

class FrostStaff : public Weapon
{
  public:
	FrostStaff();
	virtual ~FrostStaff() override;

	virtual void attack(Level *level, Entity &e, Direction dir, bool hold) override;
};