#pragma once

#include "Button.h"
#include "Tile.h"
#include "Utils.h"

class Trapdoor : public Tile
{
  protected:
	Button::State m_State;

	CollisionBox getCollisionBox();

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