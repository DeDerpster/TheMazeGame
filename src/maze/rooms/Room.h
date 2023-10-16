#pragma once

#include "Camera.h"
#include "Entity.h"
#include "Event.h"
#include "KeyDefinitions.h"
#include "Renderer.h"
#include "Tile.h"
#include "Utils.h"

#include <array>
#include <vector>

class Level;

class Room
{
  protected:
	float             x, y;
	bool              m_Entrances[4];   // 0: North 1: South 2: East 3: West
	bool               isLocked;
	RoomType           m_Type;
	Level *            m_Level;

	std::vector<Entity *> m_Entities;

	std::array<Tile *, ROOM_SIZE * ROOM_SIZE> m_Tiles;   // NOTE: Please do not store this class on the stack!

  public:
	Room(float x, float y, bool entrances[4], RoomType type, Level *level);
	virtual ~Room();
	virtual void render();
	virtual void update();
	virtual bool eventCallback(const Event::Event &e);

#ifdef DEBUG
	virtual void imGuiRender();
#endif

	Entity *entityCollisionDetection(float nextX, float nextY, CollisionBox box);

	virtual bool isOpen(Direction entrance);

	void addEntity(Entity *e) { m_Entities.push_back(e); }

	virtual void active();

	Tile *getTile(int x, int y);

	void checkForMobs();
};