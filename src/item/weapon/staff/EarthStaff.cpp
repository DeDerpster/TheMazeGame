#include "EarthStaff.h"

#include "entity/movableEntity/mob/Mob.h"
#include "layer/level/Level.h"

EarthStaff::EarthStaff()
	: Weapon("Earth Staff", 15.0f, 15, Sprite::ID::weaponEarthStaff) {}
EarthStaff::~EarthStaff() {}

void EarthStaff::attack(Level *level, Entity &e, Direction dir, bool hold)
{
	Mob *m = dynamic_cast<Mob *>(&e);
	if(m_Cooldown == 0)
	{
		float damage;
		if(m)
			damage = m->getDamage(m_Damage / 2.0f, m_Damage);
		else
			damage = 0.0f;

		// Spawns a natural projectile
		level->addProjectile(new Projectile(e.getX(), e.getY(), damage, dir, &e, level, Projectile::Type::Nature));
		if(m)
		{
			m_Cooldown = m->getWeaponDelay(m_CooldownMax);
			m->hasUsedWeapon();
		}
	}
}