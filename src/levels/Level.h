#pragma once

#include <vector>

#include "Layer.h"
#include "Utils.h"

// These are here so I don't have to import the files and cause a infinite loop
class Tile;
class Player;

class Level : public Layer
{
  public:
	Level() {}
	virtual ~Level() override {}

	virtual void render() = 0;
	virtual void update() = 0;

	virtual bool eventCallback(const Application::Event &e) = 0;
	virtual bool setEffect(const Effect::RenderEffect &e)   = 0;

	virtual Tile *  getTile(int x, int y) = 0;
	virtual Player *getPlayer() { return nullptr; }

#ifdef DEBUG
	virtual void imGuiRender() = 0;
#endif
};
