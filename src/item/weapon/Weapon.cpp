#include "Weapon.h"

Weapon::Weapon(float damage)
	: Item(), m_Damage(damage)
{
}

Weapon::Weapon(const char *name, float damage, uint16_t spriteID)
	: Item(name, spriteID), m_Damage(damage)
{
}

Weapon::~Weapon() {}