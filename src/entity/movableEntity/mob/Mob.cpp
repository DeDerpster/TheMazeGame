#include "Mob.h"

#include "item/potion/Potion.h"
#include "item/weapon/Weapon.h"

#include "Application.h"
#include "KeyDefinitions.h"
#include "layer/level/Level.h"
#include "rendering/Renderer.h"
#include "layer/MessageManager.h"

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

#define DEFINE_MY_VARS m_Inventory(DEFAULT_INVENTORY_SIZE), m_Weapons(3), m_CurrentWeapon(-1), m_Followers(1), m_Following(nullptr), m_Enemy(nullptr), m_Stats(randomStats), isInControl(false)

Mob::Mob(bool randomStats)
	: MovableEntity(0.0f, 0.0f, TILE_SIZE * 1.25f, defaultBox, nullptr, Sprite::ID::mobPlayer), DEFINE_MY_VARS
{
	setupAnimations();
}

Mob::Mob(float x, float y, bool randomStats)
	: MovableEntity(x, y, TILE_SIZE * 1.25f, defaultBox, nullptr, Sprite::ID::mobPlayer), DEFINE_MY_VARS
{
	setupAnimations();
}

Mob::Mob(float x, float y, Level *level, bool randomStats)
	: MovableEntity(x, y, TILE_SIZE * 1.25f, defaultBox, level, Sprite::ID::mobPlayer), DEFINE_MY_VARS
{
	setupAnimations();
}

Mob::Mob(float x, float y, Level *level, Sprite::ID spriteID, bool randomStats)
	: MovableEntity(x, y, TILE_SIZE * 1.25f, defaultBox, level, spriteID), DEFINE_MY_VARS
{
	setupAnimations();
}

Mob::Mob(float x, float y, float speed, Level *level, Sprite::ID spriteID, bool randomStats)
	: MovableEntity(x, y, TILE_SIZE * 1.25f, speed, Direction::south, defaultBox, level, spriteID), DEFINE_MY_VARS
{
	setupAnimations();
}

Mob::~Mob()
{
}

void Mob::render()
{
	uint8_t layer = 4;
	// If it the mob is moving it will render the animations (depending on the direction)
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
		// If not moving will only render the still frames
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
	if(!deleteMe())
	{
		// Will update the animation that is direction the mob is going in
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

		// Makes sure the followers max size is equal to the mob's attractiveness
		if(m_Followers.getMaxSize() != m_Stats.getAttractiveness())
			m_Followers.setMaxSize(m_Stats.getAttractiveness());

		// Updates the weapons
		for(Weapon *w : m_Weapons)
			w->update();

		// Updates the stats and if the stats level up and the mob is the player it will display some messages saying as much
		if(m_Stats.update() && this == m_Level->getPlayer())
		{
			MessageManager::sendMessage("You have leveled up!", MessageManager::Priority::Low);
			MessageManager::sendMessage("Your stats have increased", MessageManager::Priority::Low);
		}
	}
}

bool Mob::eventCallback(const Event::Event &e)
{
	switch(e.getType())
	{
	case Event::EventType::MobDied:
	{
		const Event::MobDiedEvent &ne = static_cast<const Event::MobDiedEvent &>(e);

		// If a mob has died it will check its following enemy and followers and removes it if that is the case
		if(ne.mob == m_Following)
			m_Following = nullptr;
		else if(ne.mob == m_Enemy)
		{
			// If the enemy has died will update the stats
			m_Stats.hasKilledTarget();
			m_Enemy = nullptr;
		}
		else
		{
			// Finds the dead follower in the out of the followers
			auto index = std::find(m_Followers.begin(), m_Followers.end(), ne.mob);
			if(index != m_Followers.end())
				m_Followers.erase(index);
		}

		if(m_Enemy)   // Will redistribute the followers roles
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

		// If the show alternate tiles been called it will distribute enemies to followers
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

	// Gets the item at the index
	Item *  item = m_Inventory[index];
	Weapon *wp   = dynamic_cast<Weapon *>(item);
	if(wp)
	{
		// If the item is a weapon it will call a transfer event
		Event::ItemTransferEvent e(index, &m_Inventory);
		Application::callEvent(e, Event::CallType::Overlay);
		return;
	}

	// If the item is a potion it will use the potion and remove it
	Potion *pt = dynamic_cast<Potion *>(item);
	if(pt)
	{
		pt->useOn(this);
		delete pt;
		m_Inventory.erase(m_Inventory.begin() + index);
	}
}

bool Mob::addFollower(Mob *follower)
{
	// If ther followers is full it will return false
	if(m_Followers.isFull())
		return false;

	// Adds the follower to the list
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

void Mob::setMaxFollowers(int max)
{
	m_Stats.setAttractiveness(max);
	m_Followers.setMaxSize(max);
}

void Mob::setFollowersEnemy(Mob *enemy)
{
	if(m_Followers.size() > 0)
	{
		// Goes through the followers and the enemy's followers and distributes them as fairly as possible
		const Container<Mob *> &eFollowers = enemy->getFollowers();

		int i = 0;
		for(Mob *follower : m_Followers)
		{
			if(i == eFollowers.size())
			{
				if(!follower->getEnemy())
					follower->setEnemy(enemy);
				// Returns to the main enemy
				i = -1;
			}
			else if(!follower->getEnemy())
				follower->setEnemy(eFollowers[i]);

			i++;
		}
	}
}

float Mob::getFollowDistance(Mob *follower)
{
	// Calculates the distance a follower should be following at
	// Finds the follower in a list
	std::vector<Mob *>::iterator index = std::find(m_Followers.begin(), m_Followers.end(), follower);
	if(index == m_Followers.end())
	{
		Log::error("Could not find follower!", LOGINFO);

		return TILE_SIZE;
	}
	else
	{
		// Gets the index of the follower as an integer and returns that
		int i = index - m_Followers.begin();
		return (i + 1) * TILE_SIZE / 2;
	}
}

void Mob::useCurrentWeapon(bool hold)
{
	// Uses the current weapon
	Weapon *weapon = m_Weapons[m_CurrentWeapon];
	weapon->attack(m_Level, *this, m_Dir, hold);
}

void Mob::setupAnimations()
{
	// Sets up the animations
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
		// If the item is a weapon and the active weapons are not full it will add it to the active weapons
		m_Weapons.push_back(weapon);
		if(m_CurrentWeapon == -1) // Updates the currect active weapon if necessary
			m_CurrentWeapon = 0;

		return true;
	}
	else if(!m_Inventory.isFull())
	{
		// Otherwise it adds it to the inventory
		m_Inventory.push_back(item);

		return true;
	}

	return false;
}