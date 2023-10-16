#pragma once

#include "Entity.h"
#include "Item.h"
#include "Text.h"
#include "Tile.h"

class WorldItem : public Entity
{
  protected:
	Item *m_Item;

  public:
	WorldItem(Item *item);
	WorldItem(float x, float y, Item *item);
	WorldItem(float x, float y, Level *level, Item *item);
	virtual ~WorldItem();

	virtual void render() override;

	virtual void update() override;
	virtual bool eventCallback(const Event::Event &e) override;
#ifdef DEBUG
	virtual void imGuiRender() override;
#endif

	Item *pickUp();

	virtual bool deleteMe() override;

	Item *peak();
};