#pragma once

#include "../MovableEntity.h"

#include "Container.h"
#include "item/Item.h"
#include "item/weapon/Weapon.h"
#include "ItemContainer.h"
#include "WeaponContainer.h"

#include "rendering/sprite/AnimatedSprite.h"
#include "stats/Stats.h"

class Mob : public MovableEntity
{
  protected:
	ItemContainer   m_Inventory;       // Stores the items in a container
	WeaponContainer m_Weapons;         // Stores current active weapons in a container
	int             m_CurrentWeapon;   // Stores the current active weapon (with -1 meaning no weapon)

	Container<Mob *> m_Followers;   // Stores list of followers
	Mob *            m_Following;   // Stores the mob it is following
	Mob *            m_Enemy;       // Stores the mob it is attacking
	Stats            m_Stats;       // Stores its stats

	// Stores the animations of the mob
	std::unique_ptr<AnimatedSprite> m_NorthAnimation;
	std::unique_ptr<AnimatedSprite> m_SouthAnimation;
	std::unique_ptr<AnimatedSprite> m_EastAnimation;
	std::unique_ptr<AnimatedSprite> m_WestAnimation;

	// Stores whether the mob is in control of the camera
	bool isInControl;

	void setupAnimations();

  public:
	Mob(bool randomStats = true);
	Mob(float x, float y, bool randomStats = true);
	Mob(float x, float y, Level *level, bool randomStats = true);
	Mob(float x, float y, Level *level, Sprite::ID spriteID, bool randomStats = true);
	Mob(float x, float y, float speed, Level *level, Sprite::ID spriteID, bool randomStats = true);
	virtual ~Mob() override;

	bool pickUp(Item *item);

	virtual void render();
	virtual void update();
	virtual bool eventCallback(const Event::Event &e);

	virtual bool deleteMe() { return m_Stats.isDead(); }

	ItemContainer *  getInventory() { return &m_Inventory; }
	WeaponContainer *getWeapons() { return &m_Weapons; }
	int *            getCurrentWeaponPointer() { return &m_CurrentWeapon; }
	void             setCurrentWeapon(int currentWeapon) { m_CurrentWeapon = currentWeapon; }
	void             useItemInInventory(uint16_t index);

	bool addFollower(Mob *follower);
	void removeFollower(Mob *follower);
	bool canAddFollower();

	virtual void setFollowing(Mob *following);
	virtual void setEnemy(Mob *enemy);

	void  setMaxFollowers(int max);
	void setFollowersEnemy(Mob *enemy);
	float getFollowDistance(Mob *follower);

	const Container<Mob *> &getFollowers() const { return m_Followers; }
	const Mob *             getFollowing() const { return m_Following; }
	const Mob *             getEnemy() const { return m_Enemy; }
	Stats *const            getStats() { return &m_Stats; }

	void useCurrentWeapon(bool hold);

	void setIsInControl(bool i_isInControl)
	{
		isInControl = i_isInControl;
	}

	void  dealDamage(float damage) { m_Stats.dealDamage(damage); }
	float getDamage(float minDamage, float maxDamage) { return m_Stats.getDamage(minDamage, maxDamage); }
	float getWeaponDelay(int delay) { return m_Stats.getWeaponDelay(delay); }
	void  hasUsedWeapon() { m_Stats.hasUsedWeapon(); }

#ifdef DEBUG
	virtual void imGuiRender() = 0;
#endif
};