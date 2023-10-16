#pragma once

#include "Item.h"
#include "Mob.h"

#include <functional>

class Potion : public Item
{
  private:
	std::function<void(Mob *)> m_EffectFunc;

  public:
	Potion(const char *name, uint32_t spriteID, std::function<void(Mob *)> effect);
	virtual ~Potion() override;

	void useOn(Mob *mob);
};