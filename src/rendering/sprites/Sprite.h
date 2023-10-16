#pragma once

#include <memory>
#include <string>

#include "Log.h"
#include "Renderer.h"
#include "Texture.h"
#include "Utils.h"

namespace Sprite
{
	class Sprite
	{
	  private:
		std::shared_ptr<Texture> m_Texture;
		std::vector<Object>      m_Buffer;

	  public:
		static std::vector<std::unique_ptr<Sprite>> sprites;

		Sprite(const char *texturePath);
		Sprite(const Sprite &sprite);
		~Sprite();

		void render(float x, float y, double rotation, float size);

		unsigned int         getSizeOfBuffer() { return m_Buffer.size(); }
		std::vector<Object> *getBuffer() { return &m_Buffer; }
		void                 clearBuffer() { m_Buffer.clear(); }

		void bind(unsigned int slot = 0);
		void unbind();

		static Sprite *getSprite(int i) { return &*sprites[i]; };
		static void    init()
		{
			sprites.push_back(std::make_unique<Sprite>("res/textures/tiles/BasicWall.png"));
			sprites.push_back(std::make_unique<Sprite>("res/textures/tiles/BasicFloor.png"));
			sprites.push_back(std::make_unique<Sprite>("res/textures/tiles/BasicCorner.png"));
			sprites.push_back(std::make_unique<Sprite>("res/textures/tiles/BasicCorner2.png"));

			sprites.push_back(std::make_unique<Sprite>("res/textures/Entities/Player-heir-north.png"));
			sprites.push_back(std::make_unique<Sprite>("res/textures/Entities/Player-heir-north-1.png"));
			sprites.push_back(std::make_unique<Sprite>("res/textures/Entities/Player-heir-north-2.png"));

			sprites.push_back(std::make_unique<Sprite>("res/textures/Entities/Player-heir-south.png"));
			sprites.push_back(std::make_unique<Sprite>("res/textures/Entities/Player-heir-south-1.png"));
			sprites.push_back(std::make_unique<Sprite>("res/textures/Entities/Player-heir-south-2.png"));

			sprites.push_back(std::make_unique<Sprite>("res/textures/Entities/Player-heir-east.png"));
			sprites.push_back(std::make_unique<Sprite>("res/textures/Entities/Player-heir-east-1.png"));
			sprites.push_back(std::make_unique<Sprite>("res/textures/Entities/Player-heir-east-2.png"));

			sprites.push_back(std::make_unique<Sprite>("res/textures/Entities/Player-heir-west.png"));
			sprites.push_back(std::make_unique<Sprite>("res/textures/Entities/Player-heir-west-1.png"));
			sprites.push_back(std::make_unique<Sprite>("res/textures/Entities/Player-heir-west-2.png"));

			sprites.push_back(std::make_unique<Sprite>("res/textures/DebugCircle.png"));
		}
	};
}   // namespace Sprite

// This is a definition for every sprite there is
#define BASIC_WALL            0
#define BASIC_FLOOR           1
#define BASIC_OUTWARDS_CORNER 2
#define BASIC_INWARDS_CORNER  3
#define PLAYER_NORTH          4
#define PLAYER_NORTH_1        5
#define PLAYER_NORTH_2        6
#define PLAYER_SOUTH          7
#define PLAYER_SOUTH_1        8
#define PLAYER_SOUTH_2        9
#define PLAYER_EAST           10
#define PLAYER_EAST_1         11
#define PLAYER_EAST_2         12
#define PLAYER_WEST           13
#define PLAYER_WEST_1         14
#define PLAYER_WEST_2         15
#define DEBUG_CIRCLE          16
#define NUM_OF_SPRITES        17