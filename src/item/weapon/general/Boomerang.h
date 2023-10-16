#pragma once

#include "../Weapon.h"

#include "Application.h"

class Boomerang : public Weapon
{
  public:
	Boomerang();
	virtual ~Boomerang() override;

	virtual void attack(Level *level, Entity &e, Direction dir, bool hold) override;
};