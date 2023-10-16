#pragma once

#include "Tile.h"
#include "gui/object/Button.h"

class Trapdoor : public Tile
{
  protected:
	Button::State m_State;   // Stores the state

	bool m_Locked;   // Allows it to be locked until the player defeats the enemies

  public:
	Trapdoor();
	Trapdoor(float x, float y, double rotation, Level *level);
	virtual ~Trapdoor() override;

	virtual void render() override;
	virtual void update() override;
	virtual bool eventCallback(const Event::Event &e) override;
#ifdef DEBUG
	virtual void imGuiRender() override;
#endif
};