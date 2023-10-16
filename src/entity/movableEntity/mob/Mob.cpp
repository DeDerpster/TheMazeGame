#include "Mob.h"
#include "Tile.h"

#include "Potion.h"
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
	: MovableEntity(0.0f, 0.0f, Tile::TILE_SIZE * 1.25f, defaultBox, nullptr, SPRITE_PLAYER), StatsMob(), m_CurrentWeapon(-1)
{
	setupAnimations();
}

Mob::Mob(float x, float y)
	: MovableEntity(x, y, Tile::TILE_SIZE * 1.25f, defaultBox, nullptr, SPRITE_PLAYER), StatsMob(), m_CurrentWeapon(-1)
{
	setupAnimations();
}

Mob::Mob(float x, float y, Level *level)
	: MovableEntity(x, y, Tile::TILE_SIZE * 1.25f, defaultBox, level, SPRITE_PLAYER), StatsMob(), m_CurrentWeapon(-1)
{
	setupAnimations();
}

Mob::Mob(float x, float y, Level *level, uint16_t spriteID)
	: MovableEntity(x, y, Tile::TILE_SIZE * 1.25f, defaultBox, level, spriteID), StatsMob(), m_CurrentWeapon(-1)
{
	setupAnimations();
}

Mob::Mob(float x, float y, float speed, Level *level, uint16_t spriteID)
	: MovableEntity(x, y, Tile::TILE_SIZE * 1.25f, speed, Direction::south, defaultBox, level, spriteID), StatsMob(), m_CurrentWeapon(-1)
{
	setupAnimations();
}

Mob::~Mob()
{
	for(Weapon *weapon : m_Weapons)
		delete weapon;
	for(Item *item : m_Inventory)
		delete item;
}

void Mob::render()
{
	if(isMoving)
	{
		switch(m_Dir)
		{
		case Direction::north:
			m_NorthAnimation->render(x, y, 0.0f, width, height);
			break;
		case Direction::south:
			m_SouthAnimation->render(x, y, 0.0f, width, height);
			break;
		case Direction::east:
			m_EastAnimation->render(x, y, 0.0f, width, height);
			break;
		default:
			m_WestAnimation->render(x, y, 0.0f, width, height);
			break;
		}
	}
	else
	{
		switch(m_Dir)
		{
		case Direction::north:
			Render::sprite(x, y, 0.0f, width, height, m_SpriteID + SPRITE_NORTH);
			break;
		case Direction::south:
			Render::sprite(x, y, 0.0f, width, height, m_SpriteID + SPRITE_SOUTH);
			break;
		case Direction::east:
			Render::sprite(x, y, 0.0f, width, height, m_SpriteID + SPRITE_EAST);
			break;
		default:
			Render::sprite(x, y, 0.0f, width, height, m_SpriteID + SPRITE_WEST);
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

void Mob::useItemInInventory(int index)
{
	if(index > m_Inventory.size())
	{
		Log::warning("Trying to access item outside of the inventory!");
		return;
	}

	Item *  item = m_Inventory[index];
	Weapon *wp   = dynamic_cast<Weapon *>(item);
	if(wp)
	{
		// TODO: Fill this in :D
		return;
	}

	Potion *pt = dynamic_cast<Potion *>(item);
	if(pt)
	{
		pt->useOn(this);
		m_Inventory.erase(m_Inventory.begin() + index);
		return;
	}
	// NOTE: This is where other stuff will go :D
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
	if(weapon && m_Weapons.size() < getMaxActiveWeapons())
	{
		m_Weapons.push_back(weapon);
		if(m_CurrentWeapon == -1)
			m_CurrentWeapon = 0;
	}
	else if(m_Inventory.size() < getMaxInventory())
		m_Inventory.push_back(item);
	else
		Log::warning("Need to actually do something here!");   // TODO: replace this
}