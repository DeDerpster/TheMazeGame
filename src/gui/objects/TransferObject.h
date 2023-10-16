#pragma once

#include "Container.h"
#include "Item.h"
#include "MenuObject.h"

class TransferObject : public MenuObject
{
  private:
	uint16_t    index;
	IContainer *container;

  public:
	TransferObject(float width, float height, Layer *layer);
	virtual ~TransferObject();

	virtual void update() override;
	virtual void render() override;
	virtual bool eventCallback(const Event::Event &e) override;

	uint16_t    getIndex() { return index; }
	IContainer *getContainer() { return container; }

	void hasTransferred();
};