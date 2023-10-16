#pragma once

#include "Container.h"

// This is the container for items that allows it to be converted to a IContainer pointer
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

	virtual Type getType() const override { return IContainer::Type::Item; }

	virtual Item *getItem(int index) const override
	{
		return Container<Item *>::operator[](index);
	}

	virtual uint16_t size() override
	{
		return Container<Item *>::size();
	}

	virtual void removeItem(int index) override
	{
		Container<Item *>::erase(Container<Item *>::begin() + index);
	}
};