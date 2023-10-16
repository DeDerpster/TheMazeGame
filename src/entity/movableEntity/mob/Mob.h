#pragma once

#include "MovableEntity.h"

#include "AnimatedSprite.h"
#include "Container.h"
#include "Item.h"
#include "ItemContainer.h"
#include "StatsMob.h"
#include "Utils.h"
#include "Weapon.h"
#include "WeaponContainer.h"

class Mob : public MovableEntity, public StatsMob
{
  protected:
	ItemContainer         m_Inventory;
	WeaponContainer       m_Weapons;
	int                   m_CurrentWeapon;

	Container<Mob *> m_Followers;
	Mob *            m_Following;
	Mob *            m_Enemy;

	std::unique_ptr<AnimatedSprite> m_NorthAnimation;
	std::unique_ptr<AnimatedSprite> m_SouthAnimation;
	std::unique_ptr<AnimatedSprite> m_EastAnimation;
	std::unique_ptr<AnimatedSprite> m_WestAnimation;

	bool isInControl = false;

	void setupAnimations();

  public:
	Mob();
	Mob(float x, float y);
	Mob(float x, float y, Level *level);
	Mob(float x, float y, Level *level, uint16_t spriteID);
	Mob(float x, float y, float speed, Level *level, uint16_t spriteID);
	virtual ~Mob() override;

	bool pickUp(Item *item);

	virtual void render();
	virtual void update();
	virtual bool eventCallback(const Event::Event &e);

	virtual bool deleteMe() { return isDead(); }

	ItemContainer *  getInventory() { return &m_Inventory; }
	WeaponContainer *getWeapons() { return &m_Weapons; }
	int *            getCurrentWeaponPointer() { return &m_CurrentWeapon; }
	void             setCurrentWeapon(int currentWeapon) { m_CurrentWeapon = currentWeapon; }
	void             useItemInInventory(uint16_t index);

	bool addFollower(Mob *follower);
	void removeFollower(Mob *follower);

	virtual void setFollowing(Mob *following);
	virtual void setEnemy(Mob *enemy);

	void setFollowersEnemy(Mob *enemy);

	const Container<Mob *> &getFollowers() const { return m_Followers; }
	const Mob *             getFollowing() const { return m_Following; }
	const Mob *             getEnemy() const { return m_Enemy; }

	void useCurrentWeapon(bool hold);

	void setIsInControl(bool i_isInControl)
	{
		isInControl = i_isInControl;
	}
#ifdef DEBUG
	virtual void imGuiRender() = 0;
#endif
};