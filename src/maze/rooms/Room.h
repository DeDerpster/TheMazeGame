#pragma once

#include "Camera.h"
#include "Renderer.h"
#include "Tile.h"
#include "Level.h"
#include "Event.h"
#include "Utils.h"
#include "KeyDefinitions.h"

#include <array>



class Room
{
  protected:
	float               x, y;
	bool              m_Entrances[4];   // 0: North 1: South 2: East 3: West
	RoomType                                m_Type;
	Level *                                 m_Level;

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

	virtual bool isOpen(Direction entrance);

	Tile *getTile(int x, int y);
};