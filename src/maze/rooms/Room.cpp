#include "Room.h"

Room::Room()
{
	m_Tiles.reserve(ROOM_SIZE * ROOM_SIZE);
}
Room::~Room() {}

void Room::render(float x, float y)
{
	for(int i = 0; i < m_Tiles.size(); i++)
	{
		m_Tiles[i].render(x, y);
	}
}
void Room::update()
{
	for(int i = 0; i < m_Tiles.size(); i++)
	{
		m_Tiles[i].update();
	}
}

bool Room::isOpen(int entrance)
{
	return m_Entrances[entrance];
}

void Room::activeRoom()
{
}

Tile *Room::getTile(int x, int y) { return &m_Tiles[y * ROOM_SIZE + x]; }

#ifdef DEBUG
void Room::imGuiRender()
{
}
#endif