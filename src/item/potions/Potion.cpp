#include "Potion.h"

Potion::Potion(const char *name, uint32_t spriteID, std::function<void(Mob *)> effect)
	: Item(name, spriteID), m_EffectFunc(effect)
{
}
// TODO: Add initializer that takes in the type of potion and creates one based off that
Potion::~Potion()
{
}

void Potion::useOn(Mob *mob)
{
	m_EffectFunc(mob);
}