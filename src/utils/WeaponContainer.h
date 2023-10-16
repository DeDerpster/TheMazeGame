#pragma once

#include "Container.h"

#include "item/weapon/Weapon.h"

class WeaponContainer : public IContainer, public Container<Weapon *>
{
  public:
	WeaponContainer(uint16_t max)
		: IContainer(), Container<Weapon *>(max) {}

	virtual ~WeaponContainer() override
	{
		for(Weapon *weapon : (*this))
			delete weapon;
	}

	virtual Type getType() const override { return IContainer::Type::Weapon; }

	virtual Item *getItem(int index) const override
	{
		return static_cast<Item *>(Container<Weapon *>::operator[](index));
	}

	virtual uint16_t size() override
	{
		return Container<Weapon *>::size();
	}

	virtual void removeItem(int index) override
	{
		Container<Weapon *>::erase(Container<Weapon *>::begin() + index);
	}
};