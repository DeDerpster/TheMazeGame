#pragma once

#include "Weapon.h"

#include "Projectile.h"

class FireStaff : public Weapon
{
  public:
	FireStaff()
		: Weapon("Fire Staff", 10.0f, ITEM_STICK) {}
	virtual ~FireStaff() {}

	virtual void attack(Level *level, Entity &e, Direction dir) override
	{
		level->addEntity(new Projectile(e.getX(), e.getY(), 10.0f, dir, &e, level));
	}
};