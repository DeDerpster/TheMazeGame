#pragma once

#include "Layer.h"

#include <vector>
#include <functional>

#include "KeyDefinitions.h"
#include "Utils.h"
#include "ItemContainer.h"

#include "Entity.h"
#include "Item.h"
#include "Player.h"
#include "Projectile.h"
#include "Tile.h"
#include "Room.h"

class Level : public Layer
{
  protected:
	int width, height;
	std::vector<Room *> m_Board;   // This stores Room * so that you can have the different subclasses of rooms also stored
	Vec2i               m_BoardOffset;

	Player                    m_Player;
	std::vector<Entity *>     m_Entities;
	std::vector<Projectile *> m_Projectiles;

#ifdef DEBUG
	bool renderAll = false;
#endif

	bool collisionPointDetection(float nextX, float nextY);
	bool collisionTileDetection(int x, int y);
	bool lineCollisionDetection(float x, float y, float xs, float ys);

	int coordsToIndex(int x, int y);

	virtual void playerDeath() = 0;

  public:
	Level(float playerStartX, float playerStartY, int width, int height, Vec2i offsetStart);
	virtual ~Level();

	virtual void render();
	virtual void update();
	virtual bool eventCallback(const Event::Event &e);

#ifdef DEBUG
	virtual void imGuiRender() = 0;
#endif

	void addRoom(int x, int y, bool entrance[4], RoomType type);
	void removeRoom(int y, int x);

	void changeXOffset(int changeBy);
	void changeYOffset(int changeBy);

	float                       getX();
	float                       getY();
	Room *                      get(int y, int x);
	int                         getMidPoint();
	Room *                      getMidRoom();
	virtual Tile *              getTile(int x, int y);
	virtual Player *            getPlayer();
	virtual std::vector<Vec2f> *getPath(Vec2f startPos, Vec2f dest, CollisionBox box);

	float convertToRelativeX(float x);
	float convertToRelativeY(float y);
	Vec2f convertToRelativePos(Vec2f pos);
	bool  isOutOfBound(float x, float y);

	virtual void addEntity(Entity *e) { m_Entities.push_back(e); }
	virtual void addProjectile(Projectile *e) { m_Projectiles.push_back(e); }

	bool            collisionDetection(float nextX, float nextY, CollisionBox box);
	bool            directionalCollision(float x, float y, float xs, float ys, CollisionBox box);
	virtual Entity *entityCollisionDetection(float nextX, float nextY, CollisionBox box);

	virtual void endLevel() = 0;
};
