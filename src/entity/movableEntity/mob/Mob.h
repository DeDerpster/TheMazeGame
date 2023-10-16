#pragma once

#include "MovableEntity.h"

#include <vector>

#include "AnimatedSprite.h"
#include "Item.h"
#include "StatsMob.h"
#include "Utils.h"

class Weapon;

class Mob : public MovableEntity, public StatsMob
{
  protected:
	std::vector<Item *>   m_Inventory;
	std::vector<Weapon *> m_Weapons;
	int                   m_CurrentWeapon;

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

	void pickUp(Item *item);   // TODO: Make this into a bool to tell it whether it can pick it up

	virtual void render();
	virtual void update();
	virtual bool eventCallback(const Event::Event &e);

	virtual bool deleteMe() { return isDead(); }

	const std::vector<Weapon *> *getWeapons() { return &m_Weapons; }
	const std::vector<Item *> *  getInventory() { return &m_Inventory; }
	int *                        getCurrentWeaponPointer() { return &m_CurrentWeapon; }
	void                         setCurrentWeapon(int currentWeapon) { m_CurrentWeapon = currentWeapon; }
	void                         useItemInInventory(int index);

	void setIsInControl(bool i_isInControl) { isInControl = i_isInControl; }
#ifdef DEBUG
	virtual void imGuiRender() = 0;
#endif
};