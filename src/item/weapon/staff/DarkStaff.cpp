#include "DarkStaff.h"

#include "entity/movableEntity/mob/Mob.h"
#include "layer/level/Level.h"

DarkStaff::DarkStaff()
	: Weapon("Dark Staff", 18.0f, 15, Sprite::ID::weaponDarkStaff) {}
DarkStaff::~DarkStaff() {}

void DarkStaff::attack(Level *level, Entity &e, Direction dir, bool hold)
{
	Mob *m = dynamic_cast<Mob *>(&e);
	if(m_Cooldown == 0)
	{
		// If it is not a mob attacking it will just do the normal damage
		float damage;
		if(m)
			damage = m->getDamage(m_Damage / 2.0f, m_Damage);
		else
			damage = m_Damage;

		// Spawns a dark projectile
		level->addProjectile(new Projectile(e.getX(), e.getY(), damage, dir, &e, level, Projectile::Type::Dark));
		if(m)
		{
			// Sets the cooldown to the mob cooldown version
			m_Cooldown = m->getWeaponDelay(m_CooldownMax);
			m->hasUsedWeapon();   // Tells the mob they have used a weapon
		}
		else
			m_Cooldown = m_CooldownMax;
	}
}