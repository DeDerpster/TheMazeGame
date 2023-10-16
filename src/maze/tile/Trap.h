#pragma once

#include "Tile.h"

class Trap : public Tile
{
  protected:
	uint16_t   m_AttackCooldown;      // Stores the timer for the cooldown
	uint16_t   m_AttackCooldownMax;   // Stores the max value of the cool down
	uint16_t   m_AttackTrapTimer;     // Stores the attack trap timer
	Sprite::ID m_AttackSpriteID;      // Stores the sprite ID of the attack position
	uint32_t   m_Damage;              // Stores the damage it does

  public:
	Trap();
	Trap(float x, float y, double rotation, Level *level);
	virtual ~Trap() override;

	virtual void render() override;
	virtual void update() override;
#ifdef DEBUG
	virtual void imGuiRender() override;
#endif
};