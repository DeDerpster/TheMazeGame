#pragma once

#include <vector>

#include "Layer.h"
#include "Utils.h"

class Tile;   // FIXME
// These are here so I don't have to import the files and cause a infinite loop
class Player;
class Entity;
class Projectile;

class Level : public Layer
{
  protected:
	int width, height;
	std::vector<Entity *> m_Entities;
	std::vector<Projectile *> m_Projectiles;

	bool collisionPointDetection(float nextX, float nextY);
	bool collisionTileDetection(int x, int y);
	bool lineCollisionDetection(float x, float y, float xs, float ys);

  public:
	Level();
	Level(int width, int height);
	virtual ~Level();

	virtual void render() = 0;
	virtual void update() = 0;
#ifdef DEBUG
	virtual void imGuiRender() = 0;
#endif

	virtual bool eventCallback(const Event::Event &e) = 0;

	virtual Tile *              getTile(int x, int y) = 0;
	virtual Player *            getPlayer() { return nullptr; }
	virtual std::vector<Vec2f> *getPath(Vec2f startPos, Vec2f dest, CollisionBox box);

	virtual void addEntity(Entity *e) { m_Entities.push_back(e); }
	virtual void addProjectile(Projectile *e) { m_Projectiles.push_back(e); }

	bool            collisionDetection(float nextX, float nextY, CollisionBox box);
	bool            directionalCollision(float x, float y, float xs, float ys, CollisionBox box);
	virtual Entity *entityCollisionDetection(float nextX, float nextY, CollisionBox box);
};
