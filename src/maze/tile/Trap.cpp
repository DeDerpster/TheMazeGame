#include "Trap.h"

#include "layer/level/Level.h"

Trap::Trap()
	: Tile(), m_AttackCooldown(0), m_AttackCooldownMax(30), m_AttackTrapTimer(0), m_AttackSpriteID(Sprite::ID::tileBasicTrapExposed), m_Damage(20)
{
}

Trap::Trap(float x, float y, double rotation, Level *level)
	: Tile(x, y, rotation, Sprite::ID::tileBasicTrapHidden, false, level), m_AttackCooldown(0), m_AttackCooldownMax(30), m_AttackTrapTimer(0), m_AttackSpriteID(Sprite::ID::tileBasicTrapExposed), m_Damage(20)
{
}

Trap::~Trap()
{
}

void Trap::render()
{
	// Either renders the attacking sprite or the normal sprite
	uint8_t layer = 0;
	if(m_AttackTrapTimer == 0)
		Render::sprite(x, y, rotation, TILE_SIZE, m_SpriteID, layer);
	else
		Render::sprite(x, y, rotation, TILE_SIZE, m_AttackSpriteID, layer);
}

void Trap::update()
{
	// Updates the timers
	if(m_AttackTrapTimer > 0)
		m_AttackTrapTimer--;

	if(m_AttackCooldown > 0)
		m_AttackCooldown--;
	else
	{
		// Checks if the player is over the trap
		// NOTE: Only works on player to make sure that you cannot easily loose a follower
		Player *const player    = m_Level->getPlayer();
		CollisionBox &playerBox = player->getCollisionBox();
		if(doesPointIntersectWithBox({player->getX(), player->getY() - TILE_SIZE / 2}, {x, y}, getCollisionBox()))
		{
			// Attacks the player
			m_AttackTrapTimer = 20;
			m_AttackCooldown  = m_AttackCooldownMax;
			player->dealDamage(m_Damage);
		}
	}
}

#ifdef DEBUG
void Trap::imGuiRender()
{
}
#endif
