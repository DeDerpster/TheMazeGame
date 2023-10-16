#pragma once

#include "Weapon.h"

#include "Application.h"
#include "Projectile.h"

class Sling : public Weapon
{
  public:
	Sling();
	virtual ~Sling() override;

	virtual void attack(Level *level, Entity &e, Direction dir, bool hold) override;
};