#pragma once

#include <vector>

#include "Layer.h"
#include "Utils.h"

#define X_STEP 25
#define Y_STEP 25

// These are here so I don't have to import the files and cause a infinite loop
class Tile;
class Player;

class Level : public Layer
{
  private:
	int width, height;

	bool collisionDetection(float nextX, float nextY);

  public:
	Level(): width(0), height(0) {}
	Level(int width, int height): width(width), height(height) {}
	virtual ~Level() override {}

	virtual void render() = 0;
	virtual void update() = 0;
#ifdef DEBUG
	virtual void imGuiRender() = 0;
#endif

	virtual bool eventCallback(const Application::Event &e) = 0;
	virtual bool setEffect(const Effect::RenderEffect &e)   = 0;

	virtual Tile *     getTile(int x, int y) = 0;
	virtual Player *   getPlayer() { return nullptr; }
	std::vector<Vec2f> getPath(Vec2f start, Vec2f destination);
};
