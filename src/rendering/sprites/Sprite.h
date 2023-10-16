#pragma once

#include <memory>
#include <string>

#include "Log.h"
#include "Renderer.h"
#include "Texture.h"
#include "Utils.h"

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
#define ITEM_STICK            16
#define DEBUG_CIRCLE          17
#define NUM_OF_SPRITES        18

namespace Render
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

			sprites.push_back(std::make_unique<Sprite>("res/textures/items/Stick.png"));

			sprites.push_back(std::make_unique<Sprite>("res/textures/DebugCircle.png"));
		}
	};

	static void spriteRender(SmartBuffer &smartBuffer)
	{
		uint32_t currentTexID = 0;   // This stores the slot the current texture is bound to, so it can set the texID part of the vertex
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
