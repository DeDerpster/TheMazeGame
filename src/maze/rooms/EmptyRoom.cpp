#include "EmptyRoom.h"

#define ROOM_PATH "res/rooms/Empty.png"

#include "Application.h"
#include "Log.h"
#include "Sprite.h"

#include "stb_image/stb_image.h"

#define _USE_MATH_DEFINES
#ifdef IS_ON_WINDOWS
	#include <math.h>
#else
	#include <cmath>
#endif

EmptyRoom::EmptyRoom(bool entrances[4])
{
	for(int i = 0; i < 4; i++)
		m_Entrances[i] = entrances[i];

	int width, height, comp;
	stbi_set_flip_vertically_on_load(1);   // This loads the bitmap file that contains the information to create the room
	unsigned char *data = stbi_load(ROOM_PATH, &width, &height, &comp, 4);

	for(int i = 0; i < height; i++)
	{
		for(int j = 0; j < width; j++)
		{   // This goes through each pixel to determine what tile will be placed
			int    texID;
			bool   isSolid  = true;
			double rotation = 0;
			// This checks if any of the entrances are closed
			if(!m_Entrances[NORTH_ENTRANCE] && i == height - 1 && j != 0 && j != width - 1)
			{
				texID = BASIC_WALL;
			}
			else if(!m_Entrances[SOUTH_ENTRANCE] && i == 0 && j != 0 && j != width - 1)
			{
				texID    = BASIC_WALL;
				rotation = M_PI;
			}
			else if(!m_Entrances[EAST_ENTRANCE] && j == width - 1 && i != 0 && i != height - 1)
			{
				texID    = BASIC_WALL;
				rotation = M_PI / 2;
			}
			else if(!m_Entrances[WEST_ENTRANCE] && j == 0 && i != 0 && i != height - 1)
			{
				texID    = BASIC_WALL;
				rotation = 3 * M_PI / 2;
			}
			else
			{
				unsigned char *pixelOffset = data + (i * width + j) * 4;
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
			m_Tiles.push_back(Tile(j, i, rotation, texID, isSolid));   // This creates the tile and adds it to the vector
		}
	}
}

EmptyRoom::~EmptyRoom() {}

#ifdef DEBUG
void EmptyRoom::imGuiRender()
{
	for(int i = 0; i < m_Tiles.size(); i++)
	{
		m_Tiles[i].imGuiRender();
	}
}
#endif