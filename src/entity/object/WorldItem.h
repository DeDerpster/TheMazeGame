#pragma once

#include "entity/Entity.h"
#include "gui/object/Button.h"
#include "item/Item.h"
#include "maze/tile/Tile.h"

class WorldItem : public Entity
{
  protected:
	Item *        m_Item;    // Stores the item
	Button::State m_State;   // Stores its state

  public:
	WorldItem(Item *item);
	WorldItem(float x, float y, Item *item);
	WorldItem(float x, float y, Level *level, Item *item);
	virtual ~WorldItem() override;

	virtual void render() override;

	virtual void update() override;
	virtual bool eventCallback(const Event::Event &e) override;
#ifdef DEBUG
	virtual void imGuiRender() override;
#endif

	// Returns the item and removes it from the world
	Item *pickUp();

	virtual bool deleteMe() override;

	virtual bool hasCollidedWith(float xs, float ys, CollisionBox box) override { return false; }

	// Allows viewing the item without picking it up
	Item *peak();
};