#pragma once

#include "entity/Entity.h"
#include "gui/object/Button.h"
#include "item/Item.h"
#include "maze/tile/Tile.h"

class WorldItem : public Entity
{
  protected:
	Item *        m_Item;
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

	virtual bool hasCollidedWith(float xs, float ys, CollisionBox box) override { return false; }

	Item *peak();
};