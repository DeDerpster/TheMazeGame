#pragma once

#include "Tile.h"

#include "ItemContainer.h"
#include "gui/object/Button.h"

class Chest : public Tile
{
  protected:
	ItemContainer m_Inventory;   // This stores the inventory of the
	Button::State m_State;       // Stores the state it is in (uses button state for simplicity)
	bool          m_IsDud;       // This is to tell if it should be a real chest (used for traps)
	bool          m_HasOpened;   // Stores whether the chest has been opened or not

	void generateInventory();

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