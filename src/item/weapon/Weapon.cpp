#include "Weapon.h"

Weapon::Weapon(float damage)
	: Item(), m_Damage(damage), m_Cooldown(0), m_CooldownMax(0)
{
}

Weapon::Weapon(const char *name, float damage, Sprite::ID spriteID)
	: Item(name, spriteID), m_Damage(damage), m_Cooldown(0), m_CooldownMax(0)
{
}

Weapon::Weapon(const char *name, float damage, uint16_t cooldown, Sprite::ID spriteID)
	: Item(name, spriteID), m_Damage(damage), m_Cooldown(0), m_CooldownMax(cooldown)
{
}

Weapon::~Weapon() {}

void Weapon::update()
{
	if(m_Cooldown != 0)
		m_Cooldown--;
}