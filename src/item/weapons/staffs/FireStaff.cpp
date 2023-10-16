#include "FireStaff.h"

#include "KeyDefinitions.h"

#include "Level.h"
#include "Mob.h"
#include "ParticleSpawner.h"

FireStaff::FireStaff()
	: Weapon("Fire Staff", 10.0f, 20, Sprite::ID::weaponFireStaff) {}
FireStaff::~FireStaff() {}

void FireStaff::attack(Level *level, Entity &e, Direction dir, bool hold)
{
	Mob *m = dynamic_cast<Mob *>(&e);
	if(m_Cooldown == 0)
	{
		float damage;
		if(m)
			damage = m->getDamage(m_Damage / 2.0f, m_Damage);
		else
			damage = 0.0f;

		level->addProjectile(new Projectile(e.getX(), e.getY(), damage, dir, &e, level, Projectile::Type::Fire));
		if(m)
		{
			m_Cooldown = m->getWeaponDelay(m_CooldownMax);
			m->hasUsedWeapon();
		}
	}
}