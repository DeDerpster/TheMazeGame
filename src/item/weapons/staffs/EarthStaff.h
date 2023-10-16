#pragma once

#include "Weapon.h"

#include "Application.h"
#include "Projectile.h"

class EarthStaff : public Weapon
{
  public:
	EarthStaff();
	virtual ~EarthStaff() override;

	virtual void attack(Level *level, Entity &e, Direction dir, bool hold) override;
};