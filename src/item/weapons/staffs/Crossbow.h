#pragma once

#include "Weapon.h"

#include "Projectile.h"
#include "Application.h"

class Crossbow : public Weapon
{
  public:
	Crossbow();
	virtual ~Crossbow() override;

	virtual void attack(Level *level, Entity &e, Direction dir, bool hold) override;
};