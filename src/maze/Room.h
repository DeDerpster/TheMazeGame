#pragma once

#include "Camera.h"
#include "Entity.h"
#include "Renderer.h"
#include "Tile.h"

#include <vector>

#define WALL_COLOUR       0xae
#define FLOOR_COLOUR      0xff
#define CORNER_OUT_COLOUR 0xcb
#define CORNER_IN_COLOUR  0x4d

#define NORTH_ENTRANCE 0
#define SOUTH_ENTRANCE 1
#define EAST_ENTRANCE  2
#define WEST_ENTRANCE  3

#define ROOM_SIZE 7

class Room
{
  protected:
	bool              m_Entrances[4];   // 0: North 1: South 2: East 3: West
	std::vector<Tile> m_Tiles;          // NOTE: Please do not store this class on the stack!

  public:
	Room()
	{
		m_Tiles.reserve(ROOM_SIZE * ROOM_SIZE);
	}
	virtual ~Room() {}
	virtual void render(float x, float y) {}
	virtual void update() {}

	virtual void imGuiRender() {}

	virtual bool isOpen(int entrance)
	{
		return m_Entrances[entrance];
	}

	Tile *getTile(int x, int y) { return &m_Tiles[y * ROOM_SIZE + x]; }
};