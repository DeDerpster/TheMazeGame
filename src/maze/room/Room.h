#pragma once

#include "KeyDefinitions.h"

#include "../tile/Tile.h"
#include "entity/Entity.h"

#include "Camera.h"
#include "CollisionBox.h"
#include "Direction.h"
#include "event/Event.h"
#include "rendering/Renderer.h"

#include <array>
#include <vector>

class Level;

class Room
{
  public:
	enum class Type : uint8_t   // Room types
	{
		Empty,
		Chest,
		Trap,
		Enemy,
		NPC,
		Exit
	};

  protected:
	float  x, y;
	bool   m_Entrances[4];   // 0: North 1: South 2: East 3: West
	bool   isLocked;
	Type   m_Type;
	Level *m_Level;

	std::vector<Entity *> m_Entities;

	std::array<Tile *, ROOM_SIZE * ROOM_SIZE> m_Tiles;

  public:
	Room(float x, float y, bool entrances[4], Type type, Level *level);
	virtual ~Room();
	virtual void render();
	virtual void update();
	virtual bool eventCallback(const Event::Event &e);

#ifdef DEBUG
	virtual void imGuiRender();
#endif

	Entity *entityCollisionDetection(float nextX, float nextY, CollisionBox box);
	void    addEntity(Entity *e) { m_Entities.push_back(e); }
	void    checkForMobs();

	virtual bool isOpen(Direction entrance);
	Tile *       getTile(int x, int y);

	virtual void active();
};