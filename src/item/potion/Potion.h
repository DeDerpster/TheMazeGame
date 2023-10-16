#pragma once

#include "../Item.h"
#include "entity/movableEntity/mob/Mob.h"

#include <functional>

class Potion : public Item
{
  public:
	// Different potion types
	enum class Type
	{
		Health,
		HealthRegen,
		HealthMagic,
		HealthHuge,

		Stamina,
		StaminaRegen,
		StaminaMagic,
		StaminaHuge,

		Book,
		MagicBook,
		Food
	};

  private:
	std::function<void(Mob *)> m_EffectFunc;

  public:
	Potion(Type type);
	Potion(const char *name, Sprite::ID spriteID, std::function<void(Mob *)> effect);
	virtual ~Potion() override;

	void useOn(Mob *mob);
};