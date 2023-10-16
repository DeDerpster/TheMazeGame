#pragma once

#include <memory>
#include <string>

#include "Log.h"
#include "Renderer.h"
#include "Texture.h"
#include "Utils.h"

#define SPRITE_ANIM_1 1
#define SPRITE_ANIM_2 2

#define SPRITE_NORTH 0
#define SPRITE_SOUTH 3
#define SPRITE_EAST  6
#define SPRITE_WEST  9

// This is a definition for every sprite there is
#define BASIC_WALL            0
#define BASIC_FLOOR           BASIC_WALL + 1
#define BASIC_OUTWARDS_CORNER BASIC_FLOOR + 1
#define BASIC_INWARDS_CORNER  BASIC_OUTWARDS_CORNER + 1

#define SPRITE_PLAYER  BASIC_INWARDS_CORNER + 1
#define PLAYER_NORTH   SPRITE_PLAYER + SPRITE_NORTH
#define PLAYER_NORTH_1 SPRITE_PLAYER + SPRITE_ANIM_1
#define PLAYER_NORTH_2 SPRITE_PLAYER + SPRITE_ANIM_2
#define PLAYER_SOUTH   SPRITE_PLAYER + SPRITE_SOUTH
#define PLAYER_SOUTH_1 PLAYER_SOUTH + SPRITE_ANIM_1
#define PLAYER_SOUTH_2 PLAYER_SOUTH + SPRITE_ANIM_1
#define PLAYER_EAST    SPRITE_PLAYER + SPRITE_EAST
#define PLAYER_EAST_1  PLAYER_EAST + SPRITE_ANIM_1
#define PLAYER_EAST_2  PLAYER_EAST + SPRITE_ANIM_2
#define PLAYER_WEST    SPRITE_PLAYER + SPRITE_WEST
#define PLAYER_WEST_1  SPRITE_WEST + SPRITE_ANIM_1
#define PLAYER_WEST_2  SPRITE_WEST + SPRITE_ANIM_2

#define ITEM_STICK PLAYER_WEST_2 + 1

#define DEBUG_CIRCLE ITEM_STICK + 1

#define NUM_OF_SPRITES DEBUG_CIRCLE + 1

namespace Render
{
	class Sprite
	{
	  private:
		std::shared_ptr<Texture> m_Texture;
		std::vector<Object>                         m_Buffer;
		static std::vector<std::unique_ptr<Sprite>> sprites;

	  public:
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
			// Tiles
			sprites.push_back(std::make_unique<Sprite>("res/textures/tiles/BasicWall.png"));
			sprites.push_back(std::make_unique<Sprite>("res/textures/tiles/BasicFloor.png"));
			sprites.push_back(std::make_unique<Sprite>("res/textures/tiles/BasicCorner.png"));
			sprites.push_back(std::make_unique<Sprite>("res/textures/tiles/BasicCorner2.png"));

			// Mobs
			sprites.push_back(std::make_unique<Sprite>("res/textures/entities/Player-heir-north.png"));
			sprites.push_back(std::make_unique<Sprite>("res/textures/entities/Player-heir-north-1.png"));
			sprites.push_back(std::make_unique<Sprite>("res/textures/entities/Player-heir-north-2.png"));

			sprites.push_back(std::make_unique<Sprite>("res/textures/entities/Player-heir-south.png"));
			sprites.push_back(std::make_unique<Sprite>("res/textures/entities/Player-heir-south-1.png"));
			sprites.push_back(std::make_unique<Sprite>("res/textures/entities/Player-heir-south-2.png"));

			sprites.push_back(std::make_unique<Sprite>("res/textures/entities/Player-heir-east.png"));
			sprites.push_back(std::make_unique<Sprite>("res/textures/entities/Player-heir-east-1.png"));
			sprites.push_back(std::make_unique<Sprite>("res/textures/entities/Player-heir-east-2.png"));

			sprites.push_back(std::make_unique<Sprite>("res/textures/entities/Player-heir-west.png"));
			sprites.push_back(std::make_unique<Sprite>("res/textures/entities/Player-heir-west-1.png"));
			sprites.push_back(std::make_unique<Sprite>("res/textures/entities/Player-heir-west-2.png"));

			// Stick
			sprites.push_back(std::make_unique<Sprite>("res/textures/items/Stick.png"));

			// Projectiles
			sprites.push_back(std::make_unique<Sprite>("res/textures/projectiles/FireProjectile.png"));
			sprites.push_back(std::make_unique<Sprite>("res/textures/DebugCircle.png"));
		}
	};

	static void spriteRender(SmartBuffer &smartBuffer)
	{
		uint16_t currentTexID = 0;   // This stores the slot the current texture is bound to, so it can set the texID part of the vertex
		for(int i = 0; i < NUM_OF_SPRITES; i++)
		{
			// Gets the buffer from the sprite
			std::vector<Render::Object> *buffer = Render::Sprite::getSprite(i)->getBuffer();
			if(buffer->size() == 0)   // Checks the buffer is not empty
				continue;

			// Binds the sprite texture to then currentTexID
			Render::Sprite::getSprite(i)->bind(currentTexID);

			// Goes through all the objects in the buffer and renders them
			for(Render::Object obj : *buffer)
			{
				// Checks if the buffer is full or the buffer is too big and draws what there is
				if(!smartBuffer.canStore(4 * sizeof(Vertex)) || currentTexID == 32)
				{
					smartBuffer.draw();
					smartBuffer.resetBuffer();   // Resets the buffer so it can draw again
					currentTexID = 0;            // resets this as all the textures have been rendered

					// Rebinds the texture so it is in the correct slot
					Sprite::getSprite(i)->unbind();
					Sprite::getSprite(i)->bind(0);
				}
				// Adds the current object to the buffer by creating its quad (this is for memory efficiency)
				auto vertices = CreateQuad(obj.x, obj.y, obj.rotation, obj.size, currentTexID);
				smartBuffer.addToBuffer((void *) &vertices, 4 * sizeof(Vertex));
			}
			// Clears the buffer of the sprite as it will all be rendered
			Sprite::getSprite(i)->clearBuffer();
			currentTexID++;
		}

		if(!smartBuffer.isEmpty())   // If the buffer is not empty, it empties it
		{
			smartBuffer.draw();
			smartBuffer.resetBuffer();
		}
	}
}   // namespace Render
