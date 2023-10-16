#pragma once

#include "Container.h"

class ItemContainer : public IContainer, public Container<Item *>
{
  public:
	ItemContainer(uint16_t max)
		: IContainer(), Container<Item *>(max) {}

	virtual ~ItemContainer() override
	{
		for(Item *item : (*this))
			delete item;
	}

	virtual Type getType() const override { return IContainer::Type::item; }

	virtual Item *getItem(int index) const override
	{
		return Container<Item *>::operator[](index);
	}

	virtual uint16_t size()
	{
		return Container<Item *>::size();
	}
};