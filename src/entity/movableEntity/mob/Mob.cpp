#include "Mob.h"

#include "Potion.h"
#include "Weapon.h"

#include "Application.h"
#include "KeyDefinitions.h"
#include "Renderer.h"

#include <math.h>

#define defaultBox                             \
	{                                          \
		{                                      \
			(float) TILE_SIZE * 0.37f,   \
			(float) -TILE_SIZE * 0.04f}, \
		{                                      \
			(float) TILE_SIZE * 0.6f,    \
				(float) TILE_SIZE * 1.0f \
		}                                      \
	}

#define DEFINE_MY_VARS m_Inventory(DEFAULT_INVENTORY_SIZE), m_Weapons(3), m_CurrentWeapon(-1)

Mob::Mob()
	: MovableEntity(0.0f, 0.0f, TILE_SIZE * 1.25f, defaultBox, nullptr, SPRITE_PLAYER), StatsMob(), DEFINE_MY_VARS
{
	setupAnimations();
}

Mob::Mob(float x, float y)
	: MovableEntity(x, y, TILE_SIZE * 1.25f, defaultBox, nullptr, SPRITE_PLAYER), StatsMob(), DEFINE_MY_VARS
{
	setupAnimations();
}

Mob::Mob(float x, float y, Level *level)
	: MovableEntity(x, y, TILE_SIZE * 1.25f, defaultBox, level, SPRITE_PLAYER), StatsMob(), DEFINE_MY_VARS
{
	setupAnimations();
}

Mob::Mob(float x, float y, Level *level, uint16_t spriteID)
	: MovableEntity(x, y, TILE_SIZE * 1.25f, defaultBox, level, spriteID), StatsMob(), DEFINE_MY_VARS
{
	setupAnimations();
}

Mob::Mob(float x, float y, float speed, Level *level, uint16_t spriteID)
	: MovableEntity(x, y, TILE_SIZE * 1.25f, speed, Direction::south, defaultBox, level, spriteID), StatsMob(), DEFINE_MY_VARS
{
	setupAnimations();
}

Mob::~Mob()
{
	/*for(Weapon *weapon : m_Weapons)
		delete weapon;
	for(Item *item : m_Inventory)
		delete item;*/
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
		static_cast<Weapon *>(w)->update();

	StatsMob::update();
}

bool Mob::eventCallback(const Event::Event &e)
{
	return MovableEntity::eventCallback(e);
}

void Mob::useItemInInventory(uint16_t index)
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
		Event::ItemTransfer e(index, &m_Inventory);
		Application::callEvent(e, true);
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

void Mob::useCurrentWeapon(bool hold)
{
	Weapon *weapon = m_Weapons[m_CurrentWeapon];
	weapon->attack(m_Level, *this, m_Dir, hold);
}

void Mob::setupAnimations()
{
	m_NorthAnimation = std::make_unique<AnimatedSprite>(ANIMATION_FRAMES, m_SpriteID + SPRITE_NORTH);
	m_SouthAnimation = std::make_unique<AnimatedSprite>(ANIMATION_FRAMES, m_SpriteID + SPRITE_SOUTH);
	m_EastAnimation  = std::make_unique<AnimatedSprite>(ANIMATION_FRAMES, m_SpriteID + SPRITE_EAST);
	m_WestAnimation  = std::make_unique<AnimatedSprite>(ANIMATION_FRAMES, m_SpriteID + SPRITE_WEST);
}

bool Mob::pickUp(Item *item)
{
	Weapon *weapon = dynamic_cast<Weapon *>(item);
	if(weapon && !m_Weapons.isFull())
	{
		m_Weapons.push_back(weapon);
		if(m_CurrentWeapon == -1)
			m_CurrentWeapon = 0;
		return true;
	}
	else if(!m_Inventory.isFull())
	{
		m_Inventory.push_back(item);
		return true;
	}

	return false;
}