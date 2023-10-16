#pragma once

#include "Tile.h"

class Trap : public Tile
{
  protected:
	uint16_t m_AttackCooldown;
	uint16_t m_AttackCooldownMax;
	uint16_t m_AttackTrapTimer;
	uint32_t m_AttackSpriteID;
	uint32_t m_Damage;

	CollisionBox getCollisionBox();

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