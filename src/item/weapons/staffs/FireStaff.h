#pragma once

#include "Weapon.h"

#include "ObjectEffect.h"
#include "Projectile.h"
#include "ParticleSpawner.h"
#include "Application.h"

class FireStaff : public Weapon
{
  public:
	FireStaff();
	virtual ~FireStaff() override;

	virtual void attack(Level *level, Entity &e, Direction dir, bool hold) override;
};