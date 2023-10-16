#pragma once

#include "Button.h"
#include "Item.h"
#include "ItemContainer.h"
#include "Tile.h"
#include "Utils.h"

class Chest : public Tile
{
  protected:
	ItemContainer       m_Inventory;
	Button::State       m_State;
	bool                m_IsDud;

	void generateInventory();

	CollisionBox getCollisionBox();

  public:
	Chest();
	Chest(float x, float y, double rotation, Level *level, bool isDud);
	virtual ~Chest() override;

	virtual void render() override;
	virtual void update() override;
	virtual bool eventCallback(const Event::Event &e) override;
#ifdef DEBUG
	virtual void imGuiRender() override;
#endif

	ItemContainer &getInventory()
	{
		return m_Inventory;
	}
};