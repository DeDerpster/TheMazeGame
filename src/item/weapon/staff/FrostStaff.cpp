#include "FrostStaff.h"

#include "entity/movableEntity/mob/Mob.h"
#include "level/Level.h"

FrostStaff::FrostStaff()
	: Weapon("Frost Staff", 15.0f, 30, Sprite::ID::weaponFrostStaff) {}
FrostStaff::~FrostStaff() {}

void FrostStaff::attack(Level *level, Entity &e, Direction dir, bool hold)
{
	Mob *m = dynamic_cast<Mob *>(&e);
	if(m_Cooldown == 0)
	{
		float damage;
		if(m)
			damage = m->getDamage(m_Damage / 2.0f, m_Damage);
		else
			damage = 0.0f;

		level->addProjectile(new Projectile(e.getX(), e.getY(), damage, dir, &e, level, Projectile::Type::Frost));
		if(m)
		{
			m_Cooldown = m->getWeaponDelay(m_CooldownMax);
			m->hasUsedWeapon();
		}
	}
}