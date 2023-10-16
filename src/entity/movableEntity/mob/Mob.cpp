#include "Mob.h"

#include "item/potion/Potion.h"
#include "item/weapon/Weapon.h"

#include "Application.h"
#include "KeyDefinitions.h"
#include "level/Level.h"
#include "rendering/Renderer.h"

#include "event/game/MobDied.h"
#include "event/game/ShowAlternatives.h"
#include "event/menu/Transfer.h"

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

#define DEFINE_MY_VARS m_Inventory(DEFAULT_INVENTORY_SIZE), m_Weapons(3), m_CurrentWeapon(-1), m_Followers(1), m_Following(nullptr), m_Enemy(nullptr)

Mob::Mob()
	: MovableEntity(0.0f, 0.0f, TILE_SIZE * 1.25f, defaultBox, nullptr, Sprite::ID::mobPlayer), StatsMob(), DEFINE_MY_VARS
{
	setupAnimations();
}

Mob::Mob(float x, float y)
	: MovableEntity(x, y, TILE_SIZE * 1.25f, defaultBox, nullptr, Sprite::ID::mobPlayer), StatsMob(), DEFINE_MY_VARS
{
	setupAnimations();
}

Mob::Mob(float x, float y, Level *level)
	: MovableEntity(x, y, TILE_SIZE * 1.25f, defaultBox, level, Sprite::ID::mobPlayer), StatsMob(), DEFINE_MY_VARS
{
	setupAnimations();
}

Mob::Mob(float x, float y, Level *level, Sprite::ID spriteID)
	: MovableEntity(x, y, TILE_SIZE * 1.25f, defaultBox, level, spriteID), StatsMob(), DEFINE_MY_VARS
{
	setupAnimations();
}

Mob::Mob(float x, float y, float speed, Level *level, Sprite::ID spriteID)
	: MovableEntity(x, y, TILE_SIZE * 1.25f, speed, Direction::south, defaultBox, level, spriteID), StatsMob(), DEFINE_MY_VARS
{
	setupAnimations();
}

Mob::~Mob()
{
}

void Mob::render()
{
	uint8_t layer = 4;
	if(isMoving)
	{
		switch(m_Dir)
		{
		case Direction::north:
			m_NorthAnimation->render(x, y, 0.0f, width, height, layer);
			break;
		case Direction::south:
			m_SouthAnimation->render(x, y, 0.0f, width, height, layer);
			break;
		case Direction::east:
			m_EastAnimation->render(x, y, 0.0f, width, height, layer);
			break;
		default:
			m_WestAnimation->render(x, y, 0.0f, width, height, layer);
			break;
		}
	}
	else
	{
		switch(m_Dir)
		{
		case Direction::north:
			Render::sprite(x, y, 0.0f, width, height, m_SpriteID + SPRITE_NORTH, layer);
			break;
		case Direction::south:
			Render::sprite(x, y, 0.0f, width, height, m_SpriteID + SPRITE_SOUTH, layer);
			break;
		case Direction::east:
			Render::sprite(x, y, 0.0f, width, height, m_SpriteID + SPRITE_EAST, layer);
			break;
		default:
			Render::sprite(x, y, 0.0f, width, height, m_SpriteID + SPRITE_WEST, layer);
			break;
		}
	}
}

void Mob::update()
{
	if(!isDead())
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
	else
	{
		Event::MobDiedEvent *e = new Event::MobDiedEvent(this);
		Application::callEventLater(e);
	}
}

bool Mob::eventCallback(const Event::Event &e)
{
	switch(e.getType())
	{
	case Event::EventType::MobDied:
	{
		const Event::MobDiedEvent &ne = static_cast<const Event::MobDiedEvent &>(e);

		if(ne.mob == m_Following)
			m_Following = nullptr;
		else if(ne.mob == m_Enemy)
			m_Enemy = nullptr;
		else
		{
			auto index = std::find(m_Followers.begin(), m_Followers.end(), ne.mob);
			if(index != m_Followers.end())
				m_Followers.erase(index);
		}

		if(m_Enemy)
		{
			setFollowersEnemy(m_Enemy);
			m_Level->getPlayer()->setFollowersEnemy(this);
		}

		return false;
	}

	case Event::EventType::ShowAltTile:
	{
		if(m_Following)
			return false;

		const Event::ShowAltTileEvent &ne = static_cast<const Event::ShowAltTileEvent &>(e);
		if(ne.showAlt)
		{
			if(m_Enemy)
			{
				setFollowersEnemy(m_Enemy);
				m_Level->getPlayer()->setFollowersEnemy(this);
			}
		}

		return false;
	}

	default:
		return MovableEntity::eventCallback(e);
	}
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
		Event::ItemTransferEvent e(index, &m_Inventory);
		Application::callEvent(e, Event::CallType::Overlay);
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

bool Mob::addFollower(Mob *follower)
{
	if(m_Followers.isFull())
		return false;

	follower->setFollowing(this);
	m_Followers.push_back(follower);
	return true;
}

void Mob::removeFollower(Mob *follower)
{
	auto index = std::find(m_Followers.begin(), m_Followers.end(), follower);
	if(index != m_Followers.end())
	{
		m_Followers.erase(index);
		follower->setFollowing(nullptr);
	}
	else
		Log::warning("Cannot find follower to remove!");
}

bool Mob::canAddFollower()
{
	return !m_Followers.isFull();
}

void Mob::setFollowing(Mob *following)
{
	m_Following = following;
}

void Mob::setEnemy(Mob *enemy)
{
	m_Enemy = enemy;
}

void Mob::setFollowersEnemy(Mob *enemy)
{
	if(m_Followers.size() > 0)
	{
		const Container<Mob *> &eFollowers = enemy->getFollowers();

		int i = 0;
		for(Mob *follower : m_Followers)
		{
			if(i == eFollowers.size())
			{
				if(!follower->getEnemy())
					follower->setEnemy(enemy);
				i = -1;
			}
			else if(!follower->getEnemy())
				follower->setEnemy(eFollowers[i]);

			i++;
		}
	}
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