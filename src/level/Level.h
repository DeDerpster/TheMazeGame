#pragma once

#include "layer/Layer.h"

#include <functional>
#include <tuple>
#include <vector>

#include "KeyDefinitions.h"
#include "Utils.h"

#include "entity/Entity.h"
#include "entity/movableEntity/mob/Player.h"
#include "entity/movableEntity/projectile/Projectile.h"
#include "maze/room/Room.h"
#include "particle/spawner/Spawner.h"

class Level : public Layer
{
  protected:
	int                 width, height;
	std::vector<Room *> m_Board;   // This stores Room * so that you can have the different subclasses of rooms also stored
	Vec2i               m_BoardOffset;

	Player                    m_Player;
	std::vector<Entity *>     m_Entities;
	std::vector<Projectile *> m_Projectiles;
	std::vector<Spawner *>    m_Spawners;

#ifdef DEBUG
	bool renderAll = false;
#endif

	bool                   collisionTileDetection(int x, int y);
	std::tuple<bool, bool> lineCollisionCheck(float x, float y, float xs, float ys, bool returnFirst = false);

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

	std::tuple<Direction, Projectile *> getDirOfProjInRange(float x, float y, float range);

	float convertToRelativeX(float x);
	float convertToRelativeY(float y);
	Vec2f convertToRelativePos(Vec2f pos);
	bool  isOutOfBound(float x, float y);

	virtual void addEntity(Entity *e) { m_Entities.push_back(e); }
	virtual void addProjectile(Projectile *e) { m_Projectiles.push_back(e); }
	virtual void addSpawner(Spawner *e) { m_Spawners.push_back(e); }

	bool                   collisionPointDetection(float nextX, float nextY);
	bool                   collisionDetection(float nextX, float nextY, CollisionBox box);
	bool                   directionalCollision(float x, float y, float xs, float ys, CollisionBox box);
	std::tuple<bool, bool> directionalCollisionCheck(float x, float y, float xs, float ys, CollisionBox box);
	virtual Entity *       entityCollisionDetection(float nextX, float nextY, CollisionBox box);

	virtual void endLevel() = 0;
};
