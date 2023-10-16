#include "Mob.h"
#include "Tile.h"

#include "Weapon.h"

#include <math.h>

#define defaultBox                             \
	{                                          \
		{                                      \
			(float) Tile::TILE_SIZE * 0.37f,   \
			(float) -Tile::TILE_SIZE * 0.04f}, \
		{                                      \
			(float) Tile::TILE_SIZE * 0.6f,    \
				(float) Tile::TILE_SIZE * 1.0f \
		}                                      \
	}

Mob::Mob()
	: MovableEntity(0.0f, 0.0f, defaultBox, nullptr, SPRITE_PLAYER), StatsMob(), m_CurrentWeapon(-1)
{
	setupAnimations();
}

Mob::Mob(float x, float y)
	: MovableEntity(x, y, defaultBox, nullptr, SPRITE_PLAYER), StatsMob(), m_CurrentWeapon(-1)
{
	setupAnimations();
}

Mob::Mob(float x, float y, Level *level)
	: MovableEntity(x, y, defaultBox, level, SPRITE_PLAYER), StatsMob(), m_CurrentWeapon(-1)
{
	setupAnimations();
}

Mob::Mob(float x, float y, Level *level, uint16_t spriteID)
	: MovableEntity(x, y, defaultBox, level, spriteID), StatsMob(), m_CurrentWeapon(-1)
{
	setupAnimations();
}

Mob::Mob(float x, float y, float speed, Level *level, uint16_t spriteID)
	: MovableEntity(x, y, speed, Direction::south, defaultBox, level, spriteID), StatsMob(), m_CurrentWeapon(-1)
{
	setupAnimations();
}

Mob::~Mob()
{
	for(Item *item : inventory)
		delete item;
}

void Mob::render()
{
#define RENDER_ARGUMENTS x, y, 0.0f, Tile::TILE_SIZE * 1.25f
	if(isMoving)
	{
		switch(m_Dir)
		{
		case Direction::north:
			m_NorthAnimation->render(RENDER_ARGUMENTS);
			break;
		case Direction::south:
			m_SouthAnimation->render(RENDER_ARGUMENTS);
			break;
		case Direction::east:
			m_EastAnimation->render(RENDER_ARGUMENTS);
			break;
		default:
			m_WestAnimation->render(RENDER_ARGUMENTS);
			break;
		}
	}
	else
	{
		switch(m_Dir)
		{
		case Direction::north:
			Render::Sprite::getSprite(m_SpriteID + SPRITE_NORTH)->render(RENDER_ARGUMENTS);
			break;
		case Direction::south:
			Render::Sprite::getSprite(m_SpriteID + SPRITE_SOUTH)->render(RENDER_ARGUMENTS);
			break;
		case Direction::east:
			Render::Sprite::getSprite(m_SpriteID + SPRITE_EAST)->render(RENDER_ARGUMENTS);
			break;
		default:
			Render::Sprite::getSprite(m_SpriteID + SPRITE_WEST)->render(RENDER_ARGUMENTS);
			break;
		}
	}
}

void Mob::update()
{
	if(isMoving)
	{
		switch(m_Dir)
		{
		case Direction::north:
			m_NorthAnimation->update();
			break;
		case Direction::south:
			m_SouthAnimation->update();
			break;
		case Direction::east:
			m_EastAnimation->update();
			break;
		default:
			m_WestAnimation->update();
			break;
		}
	}

	for(Weapon *w : m_Weapons)
		w->update();

	StatsMob::update();
}

void Mob::setupAnimations()
{
	m_NorthAnimation = std::make_unique<Render::AnimatedSprite>(2, m_SpriteID + SPRITE_NORTH);

	m_SouthAnimation = std::make_unique<Render::AnimatedSprite>(2, m_SpriteID + SPRITE_SOUTH);

	m_EastAnimation = std::make_unique<Render::AnimatedSprite>(2, m_SpriteID + SPRITE_EAST);

	m_WestAnimation = std::make_unique<Render::AnimatedSprite>(2, m_SpriteID + SPRITE_WEST);
}

void Mob::pickUp(Item *item)
{
	Weapon *weapon = dynamic_cast<Weapon *>(item);
	if(weapon)
	{
		m_Weapons.push_back(weapon);
		if(m_CurrentWeapon == -1)
			m_CurrentWeapon = 0;
	}
	else
		inventory.push_back(item);
}