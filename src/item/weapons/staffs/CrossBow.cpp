#include "Crossbow.h"

#include "KeyDefinitions.h"

#include "Level.h"
#include "Mob.h"
#include "ParticleSpawner.h"

Crossbow::Crossbow()
	: Weapon("Crossbow", 20.0f, 25, Sprite::ID::weaponCrossbow) {}
Crossbow::~Crossbow() {}

void Crossbow::attack(Level *level, Entity &e, Direction dir, bool hold)
{
	Mob *m = dynamic_cast<Mob *>(&e);
	if(m_Cooldown == 0)
	{
		float damage;
		if(m)
			damage = m->getDamage(m_Damage / 2.0f, m_Damage);
		else
			damage = 0.0f;

		level->addProjectile(new Projectile(e.getX(), e.getY(), damage, dir, &e, level, Projectile::Type::Arrow));
		if(m)
		{
			m_Cooldown = m->getWeaponDelay(m_CooldownMax);
			m->hasUsedWeapon();
		}
	}
}