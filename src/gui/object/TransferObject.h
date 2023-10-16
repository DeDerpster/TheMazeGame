#pragma once

#include "MenuObject.h"

#include "Container.h"
#include "item/Item.h"

class TransferObject : public MenuObject
{
  private:
	uint16_t    index;       // Stores the index the item is at
	IContainer *container;   // Stores the container it is at

  public:
	TransferObject(float width, float height, Layer *layer);
	virtual ~TransferObject();

	virtual void update() override;
	virtual void render() override;
	virtual bool eventCallback(const Event::Event &e) override;

	uint16_t    getIndex() { return index; }
	IContainer *getContainer() { return container; }
	Item *      getItem() { return container->getItem(index); }

	void removeItem();   // Removes the item from the container and cancels the transfer

	void hasTransferred();   // Resets variables, NOTE: The item must still be in the container, as it needs to stop the forced transfer
};