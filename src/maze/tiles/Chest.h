#pragma once

#include "Button.h"
#include "Item.h"
#include "Tile.h"
#include "Utils.h"

class Chest : public Tile
{
  protected:
	std::vector<Item *> m_Inventory;
	Button::State       m_State;

	void generateInventory();

	CollisionBox getCollisionBox();

  public:
	Chest();
	Chest(float x, float y, double rotation, Level *level);
	virtual ~Chest() override;

	virtual void render() override;
	virtual void update() override;
	virtual bool eventCallback(const Event::Event &e) override;
#ifdef DEBUG
	virtual void imGuiRender() override;
#endif

	std::vector<Item *> &getInventory()
	{
		return m_Inventory;
	}
};