#include "GoldStaff.h"

#include "entity/movableEntity/mob/Mob.h"
#include "layer/level/Level.h"

GoldStaff::GoldStaff()
	: Weapon("Gold Staff", 30.0f, 25, Sprite::ID::weaponGoldStaff) {}
GoldStaff::~GoldStaff() {}

void GoldStaff::attack(Level *level, Entity &e, Direction dir, bool hold)
{
	Mob *m = dynamic_cast<Mob *>(&e);
	if(m_Cooldown == 0)
	{
		float damage;
		if(m)
			damage = m->getDamage(m_Damage / 2.0f, m_Damage);
		else
			damage = 0.0f;

		// Spawns a gold projectile
		level->addProjectile(new Projectile(e.getX(), e.getY(), damage, dir, &e, level, Projectile::Type::Gold));
		if(m)
		{
			m_Cooldown = m->getWeaponDelay(m_CooldownMax);
			m->hasUsedWeapon();
		}
	}
}