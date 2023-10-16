#pragma once

#include <vector>

#include "Layer.h"
#include "Utils.h"

#define X_STEP 50
#define Y_STEP 50

// These are here so I don't have to import the files and cause a infinite loop
class Tile;
class Player;

class Level : public Layer
{
  protected:
	int width, height;

	bool collisionPointDetection(float nextX, float nextY);
	bool directionalCollision(float x, float y, float xs, float ys, CollisionBox collisionBox);

  public:
	Level(): width(0), height(0)
	{
	}
	Level(int width, int height): width(width), height(height) {}
	virtual ~Level() override {}

	virtual void render() = 0;
	virtual void update() = 0;
#ifdef DEBUG
	virtual void imGuiRender() = 0;
#endif

	virtual bool eventCallback(const Application::Event &e) = 0;
	virtual bool setEffect(const Effect::RenderEffect &e)   = 0;

	virtual Tile *              getTile(int x, int y) = 0;
	virtual Player *            getPlayer() { return nullptr; }
	virtual std::vector<Vec2f> *getPath(Vec2f startPos, Vec2f dest, CollisionBox collisionBox);
	bool                        collisionDetection(float nextX, float nextY, CollisionBox collisionBox);
};
