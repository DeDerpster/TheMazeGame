#include "Room.h"
#include "RoomColours.h"

#include "Application.h"
#include "Level.h"
#include "Log.h"
#include "Sprite.h"

#include "NPC.h"

#include "Chest.h"
#include "SwitchTile.h"
#include "Trap.h"
#include "Trapdoor.h"

#include <algorithm>
#include <stb_image/stb_image.h>

#define _USE_MATH_DEFINES
#ifdef IS_ON_WINDOWS
	#include <math.h>
#else
	#include <cmath>
#endif

#define ROOMS_FOLDER "res/rooms/"

Room::Room(float x, float y, bool entrances[4], RoomType type, Level *level)
	: x(x), y(y), isLocked(false), m_Type(type), m_Level(level)
{
	for(int i = 0; i < 4; i++)
		m_Entrances[i] = entrances[i];

	std::string filePath = ROOMS_FOLDER;
	if(type == RoomType::Chest)
		filePath.append("Chest.png");
	else if(type == RoomType::Trap)
		filePath.append("Trap.png");
	else if(type == RoomType::Exit)
		filePath.append("Exit.png");
	else
		filePath.append("Empty.png");

	if(type == RoomType::Enemy)
	{
		// TODO: Generate enemy
		NPC *enemy = new NPC(x + (ROOM_SIZE / 2) * TILE_SIZE, y + (ROOM_SIZE / 2) * TILE_SIZE, m_Level);
		enemy->setAttacking(m_Level->getPlayer());
		m_Entities.push_back(enemy);
	}
	else if(type == RoomType::NPC)
	{
		// TODO: Generate NPC
		NPC *npc = new NPC(x + (ROOM_SIZE / 2) * TILE_SIZE, y + (ROOM_SIZE / 2) * TILE_SIZE, m_Level);
		m_Entities.push_back(npc);
	}

	int width, height, comp;
	stbi_set_flip_vertically_on_load(1);   // This loads the bitmap file that contains the information to create the room
	unsigned char *data = stbi_load(filePath.c_str(), &width, &height, &comp, 4);

	if(height != width || height != ROOM_SIZE)
		Log::critical("Room file not configured properly!", LOGINFO);

	for(int i = 0; i < height; i++)
	{
		for(int j = 0; j < width; j++)
		{   // This goes through each pixel to determine what tile will be placed

			Vec2f pos = {x + j * TILE_SIZE, y + i * TILE_SIZE};

			unsigned char *pixelOffset = data + (i * width + j) * 4;
			// TODO: Have this programmed into the file or something nicer than this
			// This checks if any of the entrances are closed
			if(pixelOffset[0] == CHEST_COLOUR)
			{
				m_Tiles[i * ROOM_SIZE + j] = new Chest(pos.x, pos.y, 0.0f, m_Level, false);   // This creates the tile and adds it to the vector
			}
			else if(pixelOffset[0] == DUD_CHEST_COLOUR)
			{
				m_Tiles[i * ROOM_SIZE + j] = new Chest(pos.x, pos.y, 0.0f, m_Level, true);   // This creates the tile and adds it to the vector
			}
			else if(pixelOffset[0] == TRAP_COLOUR)
			{
				m_Tiles[i * ROOM_SIZE + j] = new Trap(pos.x, pos.y, 0.0f, m_Level);   // This creates the tile and adds it to the vector
			}
			else if(pixelOffset[0] == TRAPDOOR_COLOUR)
			{
				m_Tiles[i * ROOM_SIZE + j] = new Trapdoor(pos.x, pos.y, 0.0f, m_Level);   // This creates the tile and adds it to the vector
			}
			else
			{
				uint32_t    texID;
				bool   isSolid  = true;
				double rotation = 0.0f;
				bool        isSwitch = false;
				double      altRotation;

				if(!m_Entrances[Direction::north] && i == height - 1 && j != 0 && j != width - 1)
				{
					texID = BASIC_WALL;
				}
				else if(!m_Entrances[Direction::south] && i == 0 && j != 0 && j != width - 1)
				{
					texID    = BASIC_WALL;
					rotation = M_PI;
				}
				else if(!m_Entrances[Direction::east] && j == width - 1 && i != 0 && i != height - 1)
				{
					texID    = BASIC_WALL;
					rotation = M_PI / 2;
				}
				else if(!m_Entrances[Direction::west] && j == 0 && i != 0 && i != height - 1)
				{
					texID    = BASIC_WALL;
					rotation = 3 * M_PI / 2;
				}
				else
				{
					if(((j == 0 || j == width - 1) && i > height / 2 - 2 && i < height / 2 + 2) || ((i == 0 || i == height - 1) && j > width / 2 - 2 && j < width / 2 + 2))
					{
						if(j == 0)   // Makes sure that the rotation is correct
							altRotation = 3 * M_PI / 2;
						else if(j == width - 1)
							altRotation = M_PI / 2;
						else if(i == 0)
							altRotation = M_PI;
						else
							altRotation = 0.0f;
						isSwitch = true;
					}

					if(pixelOffset[0] == WALL_COLOUR)   // Checks the colour against the different defined ones
					{
						texID = BASIC_WALL;
						if(j == 0)   // Makes sure that the rotation is correct
							rotation = 3 * M_PI / 2;
						else if(j == width - 1)
							rotation = M_PI / 2;
						else if(i == 0)
							rotation = M_PI;
					}
					else if(pixelOffset[0] == FLOOR_COLOUR)
					{
						isSolid = false;
						if(i == height - 1 && !entrances[0])
							texID = BASIC_WALL;
						else
							texID = BASIC_FLOOR;
					}
					else if(pixelOffset[0] == CORNER_OUT_COLOUR)
					{
						texID = BASIC_OUTWARDS_CORNER;
						if(j <= width / 2 && i <= height / 2)
							rotation = 3 * M_PI / 2;
						else if(j > width / 2 && i > height / 2)
							rotation = M_PI / 2;
						else if(j > width / 2 && i <= height / 2)
							rotation = M_PI;
					}
					else if(pixelOffset[0] == CORNER_IN_COLOUR)
					{
						texID = BASIC_INWARDS_CORNER;
						if(j <= width / 2 && i <= height / 2)
							rotation = 3 * M_PI / 2;
						else if(j > width / 2 && i > height / 2)
							rotation = M_PI / 2;
						else if(j > width / 2 && i <= height / 2)
							rotation = M_PI;
					}
					else
						continue;   // If it is an unknown colour it continues
				}

				if(isSwitch)
				{
					m_Tiles[i * ROOM_SIZE + j] = new SwitchTile(pos.x, pos.y, rotation, texID, altRotation, BASIC_WALL, isSolid, m_Level);   // This creates the tile and adds it to the vector
				}
				else
					m_Tiles[i * ROOM_SIZE + j] = new Tile(pos.x, pos.y, rotation, texID, isSolid, m_Level);   // This creates the tile and adds it to the vector
			}
		}
	}
}
Room::~Room()
{
	for(Tile *tile : m_Tiles)
		delete tile;
	for(Entity *entity : m_Entities)
		delete entity;
}

void Room::render()
{
	for(Tile *tile : m_Tiles)
		tile->render();
	for(Entity *entity : m_Entities)
		entity->render();
}

void Room::update()
{
	for(Tile *tile : m_Tiles)
		tile->update();

	for(auto it = m_Entities.begin(); it != m_Entities.end();)
	{
		(*it)->update();
		if((*it)->deleteMe())
		{
			delete *it;
			it = m_Entities.erase(it);
		}
		else
			++it;
	}

	if(m_Type == RoomType::Enemy)
	{
		auto searchFunc = [this](const Entity *o) -> bool {
			const NPC *npc = dynamic_cast<const NPC *>(o);
			return npc && npc->getAttacking() == m_Level->getPlayer();
		};
		std::vector<Entity *>::iterator it = std::find_if(m_Entities.begin(), m_Entities.end(), searchFunc);
		if(it == m_Entities.end())
		{
			Event::ShowAltTileEvent e(false);
			Application::callEvent(e);
		}
	}
}

bool Room::eventCallback(const Event::Event &e)
{
	bool moveEntity = false;

	// TODO: Make all these case switch statements
	if(m_Type == RoomType::Enemy && e.getType() == Event::EventType::showAltTileEvent)
	{
		const Event::ShowAltTileEvent &ne = static_cast<const Event::ShowAltTileEvent &>(e);
		if(!ne.showAlt)
		{
			m_Type   = RoomType::Empty;
			isLocked = false;
		}
	}
	else if(e.getType() == Event::EventType::mazeMovedEvent)
	{
		const Event::MazeMovedEvent &ne = static_cast<const Event::MazeMovedEvent &>(e);
		x += ne.changeX;
		y += ne.changeY;
	}
	else if(e.getType() == Event::EventType::playerResponse)
	{
		const Event::PlayerResponse &ne = static_cast<const Event::PlayerResponse &>(e);

		if(ne.response == Event::PlayerResponse::accept)
			moveEntity = true;
	}

	for(Tile *tile : m_Tiles)
	{
		if(tile->eventCallback(e))
			return true;
	}

	for(int i = 0; i < m_Entities.size(); i++)
	{
		if(m_Entities[i]->eventCallback(e))
		{
			if(moveEntity)
			{
				m_Level->addEntity(m_Entities[i]);

				m_Entities.erase(m_Entities.begin() + i);
			}

			return true;
		}
	}

	return false;
}

bool Room::isOpen(Direction entrance)
{
	return !isLocked && m_Entrances[entrance];
}

void Room::active()
{
	if(m_Type == RoomType::Enemy)
	{
		Event::ShowAltTileEvent e(true);
		Application::callEvent(e);
		isLocked = true;
	}
}

Entity *Room::entityCollisionDetection(float nextX, float nextY, CollisionBox box)
{
	for(Entity *e : m_Entities)
	{
		if(e->hasCollidedWith(nextX, nextY, box))
			return e;
	}

	return nullptr;
}

Tile *Room::getTile(int x, int y) { return m_Tiles[y * ROOM_SIZE + x]; }

#ifdef DEBUG
void Room::imGuiRender()
{
}

#endif