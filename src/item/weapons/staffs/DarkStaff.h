#pragma once

#include "Weapon.h"

#include "Projectile.h"
#include "Application.h"

class DarkStaff : public Weapon
{
  public:
	DarkStaff();
	virtual ~DarkStaff() override;

	virtual void attack(Level *level, Entity &e, Direction dir, bool hold) override;
};