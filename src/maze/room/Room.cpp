#include "Room.h"
#include "RoomColours.h"

#include "Application.h"
#include "Log.h"
#include "RandomGen.h"
#include "layer/level/Level.h"
#include "rendering/sprite/Sprite.h"

#include "entity/movableEntity/mob/NPC.h"

#include "../tile/Chest.h"
#include "../tile/SwitchTile.h"
#include "../tile/Trap.h"
#include "../tile/Trapdoor.h"

#include "event/game/MazeMoved.h"
#include "event/game/MobDied.h"
#include "event/game/PlayerResponse.h"
#include "event/game/ShowAlternatives.h"

#include <algorithm>
#include <stb_image/stb_image.h>

#define _USE_MATH_DEFINES
#ifdef IS_ON_WINDOWS
	#include <math.h>
#else
	#include <cmath>
#endif

#define ROOMS_FOLDER "res/rooms/"

Room::Room(float x, float y, bool entrances[4], Type type, Level *level)
	: x(x), y(y), isLocked(false), m_Type(type), m_Level(level)
{
	// Sets the entrances
	for(int i = 0; i < 4; i++)
		m_Entrances[i] = entrances[i];

	// Determines what file to look at based on the type
	std::string filepath = ROOMS_FOLDER;
	switch(type)
	{
	case Type::Chest:
		filepath.append("Chest.png");
		break;
	case Type::Trap:
		filepath.append("Trap.png");

		break;
	case Type::Exit:
	{
		filepath.append("Exit.png");
		// If the room is the exit it generates an enemy with 2 followers
		NPC *enemy = new NPC(x + (ROOM_SIZE / 2) * TILE_SIZE, y + (ROOM_SIZE / 2) * TILE_SIZE, m_Level, NPC::Type::Enemy);
		enemy->setMaxFollowers(2);

		enemy->setEnemy(m_Level->getPlayer());
		m_Entities.push_back(enemy);

		for(int i = 0; i < 2; i++)
		{
			if(enemy->canAddFollower())
			{
				NPC *follower = new NPC(x + (ROOM_SIZE / 2) * TILE_SIZE, y + (ROOM_SIZE / 2) * TILE_SIZE, m_Level, NPC::Type::Follower);
				enemy->addFollower(follower);
				m_Entities.push_back(follower);
			}
		}

		break;
	}

	case Type::Enemy:
	{
		filepath.append("Empty.png");

		// If the room is an enemy it generates an enemy with a random number of followers
		NPC *enemy = new NPC(x + (ROOM_SIZE / 2) * TILE_SIZE, y + (ROOM_SIZE / 2) * TILE_SIZE, m_Level, NPC::Type::Enemy);
		enemy->setEnemy(m_Level->getPlayer());
		m_Entities.push_back(enemy);
		while(enemy->canAddFollower())
		{
			int r = Random::uniformDist(0, 4);
			if(r = 0)
			{
				NPC *follower = new NPC(x + (ROOM_SIZE / 2) * TILE_SIZE, y + (ROOM_SIZE / 2) * TILE_SIZE, m_Level, NPC::Type::Follower);
				enemy->addFollower(follower);
				m_Entities.push_back(follower);
			}
			else
				break;
		}

		break;
	}

	case Type::NPC:
	{
		// Generates a random NPC
		NPC *npc = new NPC(x + (ROOM_SIZE / 2) * TILE_SIZE, y + (ROOM_SIZE / 2) * TILE_SIZE, m_Level, NPC::Type::Follower);
		m_Entities.push_back(npc);
	}

	default:
		filepath.append("Empty.png");
		break;
	}

	// Loads the filepath for the room
	int width, height, comp;
	stbi_set_flip_vertically_on_load(1);   // This loads the bitmap file that contains the information to create the room
	unsigned char *data = stbi_load(filepath.c_str(), &width, &height, &comp, 4);

	if(height != width || height != ROOM_SIZE)
		Log::critical("Room file not configured properly!", LOGINFO);

	for(int i = 0; i < height; i++)
	{
		for(int j = 0; j < width; j++)
		{   // This goes through each pixel to determine what tile will be placed

			Vec2f pos = {x + j * TILE_SIZE, y + i * TILE_SIZE};

			unsigned char *pixelOffset = data + (i * width + j) * 4;

			// This checks if any of the entrances are closed
			switch(pixelOffset[0])
			{
			case CHEST_COLOUR:
				m_Tiles[i * ROOM_SIZE + j] = new Chest(pos.x, pos.y, 0.0f, m_Level, false);   // This creates the tile and adds it to the vector
				break;
			case DUD_CHEST_COLOUR:
				m_Tiles[i * ROOM_SIZE + j] = new Chest(pos.x, pos.y, 0.0f, m_Level, true);   // This creates the tile and adds it to the vector
				break;
			case TRAP_COLOUR:
				m_Tiles[i * ROOM_SIZE + j] = new Trap(pos.x, pos.y, 0.0f, m_Level);   // This creates the tile and adds it to the vector
				break;
			case TRAPDOOR_COLOUR:
				m_Tiles[i * ROOM_SIZE + j] = new Trapdoor(pos.x, pos.y, 0.0f, m_Level);   // This creates the tile and adds it to the vector
				break;
			default:
			{
				Sprite::ID texID;
				bool       isSolid  = true;
				double     rotation = 0.0f;
				bool       isSwitch = false;
				double     altRotation;

				// If the entrances are closed it will force the tiles to be walls
				if(!m_Entrances[Direction::north] && i == height - 1 && j != 0 && j != width - 1)
				{
					texID = Sprite::ID::tileBasicWall;
				}
				else if(!m_Entrances[Direction::south] && i == 0 && j != 0 && j != width - 1)
				{
					texID    = Sprite::ID::tileBasicWall;
					rotation = M_PI;
				}
				else if(!m_Entrances[Direction::east] && j == width - 1 && i != 0 && i != height - 1)
				{
					texID    = Sprite::ID::tileBasicWall;
					rotation = M_PI / 2;
				}
				else if(!m_Entrances[Direction::west] && j == 0 && i != 0 && i != height - 1)
				{
					texID    = Sprite::ID::tileBasicWall;
					rotation = 3 * M_PI / 2;
				}
				else
				{
					// If in the correct spot it will create a switch tile
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

					switch(pixelOffset[0])
					{   // Checks against all the different colours to file the tile it is supposed to be
					case WALL_COLOUR:
						texID = Sprite::ID::tileBasicWall;
						if(j == 0)   // Makes sure that the rotation is correct
							rotation = 3 * M_PI / 2;
						else if(j == width - 1)
							rotation = M_PI / 2;
						else if(i == 0)
							rotation = M_PI;

						break;

					case FLOOR_COLOUR:
						isSolid = false;
						texID = Sprite::ID::tileBasicFloor;

						break;

					case CORNER_OUT_COLOUR:
						texID = Sprite::ID::tileBasicExtCorner;
						// Makes sure the rotation is correct
						if(j <= width / 2 && i <= height / 2)
							rotation = 3 * M_PI / 2;
						else if(j > width / 2 && i > height / 2)
							rotation = M_PI / 2;
						else if(j > width / 2 && i <= height / 2)
							rotation = M_PI;

						break;

					case CORNER_IN_COLOUR:
						texID = Sprite::ID::tileBasicIntCorner;
						// Makes sure the rotation is correct
						if(j <= width / 2 && i <= height / 2)
							rotation = 3 * M_PI / 2;
						else if(j > width / 2 && i > height / 2)
							rotation = M_PI / 2;
						else if(j > width / 2 && i <= height / 2)
							rotation = M_PI;

						break;

					default:
						Log::warning("Unknown colour when reading room file!");
						continue;   // If it is an unknown colour it continues
					}
				}

				if(isSwitch)
				{
					m_Tiles[i * ROOM_SIZE + j] = new SwitchTile(pos.x, pos.y, rotation, texID, altRotation, Sprite::ID::tileBasicWall, isSolid, m_Level);   // This creates the tile and adds it to the vector
				}
				else
					m_Tiles[i * ROOM_SIZE + j] = new Tile(pos.x, pos.y, rotation, texID, isSolid, m_Level);   // This creates the tile and adds it to the vector
				break;
			}
			}
		}
	}
}
Room::~Room()
{
	// Deletes the tiles and entities it is storing
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
	// Updates all the tiles
	for(Tile *tile : m_Tiles)
		tile->update();

	// Updates all the enemies and will delete the dead entities
	for(auto it = m_Entities.begin(); it != m_Entities.end();)
	{
		(*it)->update();
		if((*it)->deleteMe())
		{
			Mob *mob = dynamic_cast<Mob *>(*it);
			if(mob)
			{
				Application::callEventLater(new Event::MobDiedEvent(mob));
				++it;
			}
			else
			{
				delete *it;
				it = m_Entities.erase(it);
			}
		}
		else
			++it;
	}
}

bool Room::eventCallback(const Event::Event &e)
{
	bool moveEntity = false;

	// Checks for a player response (as it will have to move the follower into the maze)
	if(e.getType() == Event::EventType::PlayerResponse)
	{
		const Event::PlayerResponseEvent &ne = static_cast<const Event::PlayerResponseEvent &>(e);

		if(ne.response == Event::PlayerResponseEvent::Response::Accept)
			moveEntity = true;
	}

	// Calls the event on all the tiles
	for(Tile *tile : m_Tiles)
	{
		if(tile->eventCallback(e))
			return true;
	}

	// Calls the event on the entities and moves the entity if needed
	for(int i = 0; i < m_Entities.size(); i++)
	{
		if(m_Entities[i]->eventCallback(e))
		{
			// NOTE: It is not checking for a mob only whether it has responded to the player response
			if(moveEntity)
			{
				m_Level->addEntity(m_Entities[i]);

				m_Entities.erase(m_Entities.begin() + i);
			}

			return true;
		}
	}

	switch(e.getType())
	{
	case Event::EventType::ShowAltTile:   // If show alt tiles is called it will update the lock and tile
	{
		const Event::ShowAltTileEvent &ne = static_cast<const Event::ShowAltTileEvent &>(e);

		if(ne.showAlt)
		{
			m_Type   = Type::Enemy;   // This is to allow features of an enemy room when a follower attacks
			isLocked = true;
		}
		else
		{
			m_Type   = Type::Empty;
			isLocked = false;
		}

		return false;
	}

	case Event::EventType::MazeMoved:
	{
		// Updates its position
		const Event::MazeMovedEvent &ne = static_cast<const Event::MazeMovedEvent &>(e);

		x += ne.changeX;
		y += ne.changeY;

		return false;
	}

	case Event::EventType::MobDied:
	{
		const Event::MobDiedEvent &ne = static_cast<const Event::MobDiedEvent &>(e);

		// Finds the mob in its entity list and removes it and deletes it
		auto index = std::find(m_Entities.begin(), m_Entities.end(), ne.mob);
		if(index != m_Entities.end())
		{
			delete ne.mob;
			m_Entities.erase(index);
			checkForMobs();   // Checks what mobs are left - if it is an enemy room and there are no mobs left it will open the entrances
		}

		return false;
	}

	default:
		return false;
	}
}

bool Room::isOpen(Direction entrance)
{
	// Returns false if it is locked or the entrances state it is open
	return !isLocked && m_Entrances[entrance];
}

void Room::active()
{
	if(m_Type == Type::Enemy || m_Type == Type::Exit)
	{   // If it is an enemy room it calls a ShowAltTileEvent
		Event::ShowAltTileEvent e(true);
		Application::callEvent(e);
	}
}

Entity *Room::entityCollisionDetection(float nextX, float nextY, CollisionBox box)
{
	// Checks all the entities in the room if it has collided with it
	for(Entity *e : m_Entities)
	{
		if(e->hasCollidedWith(nextX, nextY, box))
			return e;
	}

	return nullptr;
}

Tile *Room::getTile(int x, int y) { return m_Tiles[y * ROOM_SIZE + x]; }

void Room::checkForMobs()
{
	if(isLocked)
	{   // If it is locked it will check if any mobs are left
		auto searchFunc = [this](const Entity *o) -> bool {
			const Mob *mob = dynamic_cast<const Mob *>(o);
			return mob && mob->getEnemy() == m_Level->getPlayer();
		};
		std::vector<Entity *>::iterator it = std::find_if(m_Entities.begin(), m_Entities.end(), searchFunc);
		if(it == m_Entities.end())
		{
			Event::ShowAltTileEvent e(false);
			Application::callEvent(e);
		}
	}
}

#ifdef DEBUG

void Room::imGuiRender()
{
}

#endif