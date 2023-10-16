#pragma once

#include "layer/Layer.h"

#include <functional>
#include <tuple>
#include <vector>

#include "2dVec.h"
#include "CollisionBox.h"
#include "Direction.h"
#include "KeyDefinitions.h"

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

  public:
	Level(float playerStartX, float playerStartY, int width, int height, Vec2i offsetStart);
	virtual ~Level();

	virtual void render();
	virtual void update();
	virtual bool eventCallback(const Event::Event &e);

#ifdef DEBUG
	virtual void imGuiRender() = 0;
#endif

	void addRoom(int x, int y, bool entrance[4], Room::Type type);
	void removeRoom(int y, int x);

	// Getters
	int const                 getMidPoint() const;
	float const               getX() const;
	float const               getY() const;
	Room *const               get(int y, int x) const;
	Room *const               getMidRoom() const;
	virtual const Tile *const getTile(int x, int y) const;
	virtual Player *const     getPlayer();

	// This function is the function links all all the coordinate systems together to get the path as efficiently as possible
	virtual std::vector<Vec2f> *getPath(Vec2f startPos, Vec2f dest, CollisionBox box);

	// Returns the closest projectile in a range
	Projectile *getProjInRange(float x, float y, float range);

	// Conversion algorithms
	float convertToRelativeX(float x);
	float convertToRelativeY(float y);
	Vec2f convertToRelativePos(Vec2f pos);
	bool  isOutOfBound(float x, float y);

	// Allows the addition of entities, projectiles and spawners to the level
	virtual void addEntity(Entity *e) { m_Entities.push_back(e); }
	virtual void addProjectile(Projectile *e) { m_Projectiles.push_back(e); }
	virtual void addSpawner(Spawner *e) { m_Spawners.push_back(e); }
	virtual void addEntityToRoom(Entity *e);

	// Collision detection algorithms
	virtual Entity *entityCollisionDetection(float nextX, float nextY, CollisionBox box);   // Returns entity that a box has collided with
	bool            collisionPointDetection(float nextX, float nextY);                      // Returns whether a point has had a collision
	bool            collisionDetection(float nextX, float nextY, CollisionBox box);         // Returns whether a box has had a collision

	// Directional collision detection
	bool                   directionalCollision(float x, float y, float xs, float ys, CollisionBox box);        // Returns if a box will collide with something while moving
	std::tuple<bool, bool> directionalCollisionCheck(float x, float y, float xs, float ys, CollisionBox box);   // Returns the whether a box will collide in the x direction or the y direction

	virtual void endLevel() = 0;

  private:
	bool                   collisionTileDetection(int tileX, int tileY);
	std::tuple<bool, bool> lineCollisionCheck(float x, float y, float xs, float ys, bool returnFirst = false);   // Follows a line created by the xs and ys to see if at any point it collides with a tile

	int const coordsToIndex(int x, int y) const;

	virtual void playerDeath() = 0;

	// As the A* algorithm uses multiple coordinate systems, each function is designed to handle one coordinate system and return the paths
	virtual Vec2f               getNextRoom(Vec2f relativeStart, Vec2f relativeDest);                                 // Runs the A* algorithm on the rooms to get the next room in the path (returning the relative position)
	virtual std::vector<Vec2f> *getNodePath(Vec2f relativeStart, Vec2f relativeDest, CollisionBox box);               // Runs the A* algorithm using the node coordinate system
	virtual std::vector<Vec2f> *getGridPath(Vec2i startNode, Vec2i destNode, Vec2f relativeDest, CollisionBox box);   // Runs the A* algorithm on a custom node grid of set size to increase efficiency NOTE: This returns relative position of coordinate not node position

  protected:
	// Changing the offsets of the level
	void changeXOffset(int changeBy);
	void changeYOffset(int changeBy);
};
