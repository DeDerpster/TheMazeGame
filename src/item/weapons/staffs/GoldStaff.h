#pragma once

#include "Weapon.h"

#include "Application.h"
#include "Projectile.h"

class GoldStaff : public Weapon
{
  public:
	GoldStaff();
	virtual ~GoldStaff() override;

	virtual void attack(Level *level, Entity &e, Direction dir, bool hold) override;
};