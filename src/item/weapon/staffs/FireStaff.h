#pragma once

#include "Weapon.h"

#include "Projectile.h"

class FireStaff : public Weapon
{
  public:
	FireStaff()
		: Weapon("Fire Staff", 10.0f, 20, ITEM_STICK) {}
	virtual ~FireStaff() {}

	virtual void attack(Level *level, Mob &e, Direction dir, bool hold) override
	{
		if(m_Cooldown == 0)
		{
			float        damage = e.getDamage(0.0f, m_Damage);
			CollisionBox box    = {{25, 20},
                                {60, 20}};
			float        speed  = 12.5f;
			level->addProjectile(new Projectile(e.getX(), e.getY(), damage, speed, dir, &e, level, box));
			m_Cooldown = e.getWeaponDelay(m_CooldownMax);
			e.hasUsedWeapon();
		}
	}
};