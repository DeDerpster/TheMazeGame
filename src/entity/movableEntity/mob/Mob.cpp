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
	: MovableEntity(0.0f, 0.0f, defaultBox, nullptr, SPRITE_PLAYER), StatsMob(), m_CurrentWeapon(-1), m_Size(Tile::TILE_SIZE * 1.25f)
{
	setupAnimations();
}

Mob::Mob(float x, float y)
	: MovableEntity(x, y, defaultBox, nullptr, SPRITE_PLAYER), StatsMob(), m_CurrentWeapon(-1), m_Size(Tile::TILE_SIZE * 1.25f)
{
	setupAnimations();
}

Mob::Mob(float x, float y, Level *level)
	: MovableEntity(x, y, defaultBox, level, SPRITE_PLAYER), StatsMob(), m_CurrentWeapon(-1), m_Size(Tile::TILE_SIZE * 1.25f)
{
	setupAnimations();
}

Mob::Mob(float x, float y, Level *level, uint16_t spriteID)
	: MovableEntity(x, y, defaultBox, level, spriteID), StatsMob(), m_CurrentWeapon(-1), m_Size(Tile::TILE_SIZE * 1.25f)
{
	setupAnimations();
}

Mob::Mob(float x, float y, float speed, Level *level, uint16_t spriteID)
	: MovableEntity(x, y, speed, Direction::south, defaultBox, level, spriteID), StatsMob(), m_CurrentWeapon(-1), m_Size(Tile::TILE_SIZE * 1.25f)
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
	if(isMoving)
	{
		switch(m_Dir)
		{
		case Direction::north:
			m_NorthAnimation->render(x, y, 0.0f, m_Size);
			break;
		case Direction::south:
			m_SouthAnimation->render(x, y, 0.0f, m_Size);
			break;
		case Direction::east:
			m_EastAnimation->render(x, y, 0.0f, m_Size);
			break;
		default:
			m_WestAnimation->render(x, y, 0.0f, m_Size);
			break;
		}
	}
	else
	{
		switch(m_Dir)
		{
		case Direction::north:
			Render::sprite(x, y, 0.0f, m_Size, m_SpriteID + SPRITE_NORTH);
			break;
		case Direction::south:
			Render::sprite(x, y, 0.0f, m_Size, m_SpriteID + SPRITE_SOUTH);
			// Sprite::getSprite(m_SpriteID + SPRITE_SOUTH)->render(RENDER_ARGUMENTS);
			break;
		case Direction::east:
			Render::sprite(x, y, 0.0f, m_Size, m_SpriteID + SPRITE_EAST);
			// Sprite::getSprite(m_SpriteID + SPRITE_EAST)->render(RENDER_ARGUMENTS);
			break;
		default:
			Render::sprite(x, y, 0.0f, m_Size, m_SpriteID + SPRITE_WEST);
			// Sprite::getSprite(m_SpriteID + SPRITE_WEST)->render(RENDER_ARGUMENTS);
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

bool Mob::eventCallback(const Event::Event &e)
{
	return MovableEntity::eventCallback(e);
}

void Mob::setupAnimations()
{
	m_NorthAnimation = std::make_unique<AnimatedSprite>(2, m_SpriteID + SPRITE_NORTH);

	m_SouthAnimation = std::make_unique<AnimatedSprite>(2, m_SpriteID + SPRITE_SOUTH);

	m_EastAnimation = std::make_unique<AnimatedSprite>(2, m_SpriteID + SPRITE_EAST);

	m_WestAnimation = std::make_unique<AnimatedSprite>(2, m_SpriteID + SPRITE_WEST);
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