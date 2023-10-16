#include "Room.h"
#include "RoomColours.h"

#include "Application.h"
#include "Log.h"
#include "Sprite.h"

#include "Chest.h"

#include "stb_image/stb_image.h"

#define _USE_MATH_DEFINES
#ifdef IS_ON_WINDOWS
	#include <math.h>
#else
	#include <cmath>
#endif

#define ROOMS_FOLDER "res/rooms/"

Room::Room(float x, float y, bool entrances[4], RoomType type, Level *level)
	: x(x), y(y), m_Type(type), m_Level(level)
{
	for(int i = 0; i < 4; i++)
		m_Entrances[i] = entrances[i];

	std::string filePath = ROOMS_FOLDER;
	if(type == RoomType::Chest)
		filePath.append("Chest.png");
	else if(type == RoomType::Trap)
		filePath.append("Trap.png");
	else
		filePath.append("Empty.png");

	int width, height, comp;
	stbi_set_flip_vertically_on_load(1);   // This loads the bitmap file that contains the information to create the room
	unsigned char *data = stbi_load(filePath.c_str(), &width, &height, &comp, 4);

	for(int i = 0; i < height; i++)
	{
		for(int j = 0; j < width; j++)
		{   // This goes through each pixel to determine what tile will be placed

			unsigned char *pixelOffset = data + (i * width + j) * 4;
			// TODO: Have this programmed into the file or something nicer than this
			// This checks if any of the entrances are closed
			if(pixelOffset[0] == CHEST_COLOUR)
			{
				m_Tiles[i * ROOM_SIZE + j] = new Chest(x + j * TILE_SIZE, y + i * TILE_SIZE, 0.0f, m_Level);   // This creates the tile and adds it to the vector
			}
			else
			{
				int    texID;
				bool   isSolid  = true;
				double rotation = 0.0f;
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

				m_Tiles[i * ROOM_SIZE + j] = new Tile(x + j * TILE_SIZE, y + i * TILE_SIZE, rotation, texID, isSolid, m_Level);   // This creates the tile and adds it to the vector
			}
		}
	}
}
Room::~Room()
{
	for(Tile *tile : m_Tiles)
		delete tile;
}

void Room::render()
{
	for(Tile *tile : m_Tiles)
		tile->render();
}

void Room::update()
{
	for(Tile *tile : m_Tiles)
		tile->update();
}

bool Room::eventCallback(const Event::Event &e)
{

	if(e.getType() == Event::EventType::mazeMovedEvent)
	{
		const Event::MazeMovedEvent &ne = static_cast<const Event::MazeMovedEvent &>(e);
		x += ne.changeX;
		y += ne.changeY;
	}

	for(Tile *tile : m_Tiles)
	{
		if(tile->eventCallback(e))
			return true;
	}

	return false;
}

bool Room::isOpen(Direction entrance)
{
	return m_Entrances[entrance];
}

Tile *Room::getTile(int x, int y) { return m_Tiles[y * ROOM_SIZE + x]; }

#ifdef DEBUG
void Room::imGuiRender()
{
}
#endif