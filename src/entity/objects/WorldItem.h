#pragma once

#include "Button.h"
#include "Entity.h"
#include "Item.h"
#include "Tile.h"

class WorldItem : public Entity
{
  protected:
	Item *m_Item;
	Button::State m_State;

  public:
	WorldItem(Item *item);
	WorldItem(float x, float y, float size, Item *item);
	WorldItem(float x, float y, float size, Level *level, Item *item);
	virtual ~WorldItem() override;

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