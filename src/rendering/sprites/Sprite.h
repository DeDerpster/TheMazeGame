#pragma once

#include <array>
#include <memory>
#include <string>

#include "Log.h"
#include "RenderObject.h"
#include "Texture.h"
#include "Utils.h"

#define SPRITE_ANIM_1 1
#define SPRITE_ANIM_2 2

#define SPRITE_NORTH 0
#define SPRITE_SOUTH 3
#define SPRITE_EAST  6
#define SPRITE_WEST  9

// This is a definition for every sprite there is
// Tiles
#define BASIC_WALL            0
#define BASIC_FLOOR           BASIC_WALL + 1
#define BASIC_OUTWARDS_CORNER BASIC_FLOOR + 1
#define BASIC_INWARDS_CORNER  BASIC_OUTWARDS_CORNER + 1

// Mobs
#define SPRITE_PLAYER  BASIC_INWARDS_CORNER + 1
#define PLAYER_NORTH   SPRITE_PLAYER + SPRITE_NORTH
#define PLAYER_NORTH_1 SPRITE_PLAYER + SPRITE_ANIM_1
#define PLAYER_NORTH_2 SPRITE_PLAYER + SPRITE_ANIM_2
#define PLAYER_SOUTH   SPRITE_PLAYER + SPRITE_SOUTH
#define PLAYER_SOUTH_1 PLAYER_SOUTH + SPRITE_ANIM_1
#define PLAYER_SOUTH_2 PLAYER_SOUTH + SPRITE_ANIM_2
#define PLAYER_EAST    SPRITE_PLAYER + SPRITE_EAST
#define PLAYER_EAST_1  PLAYER_EAST + SPRITE_ANIM_1
#define PLAYER_EAST_2  PLAYER_EAST + SPRITE_ANIM_2
#define PLAYER_WEST    SPRITE_PLAYER + SPRITE_WEST
#define PLAYER_WEST_1  PLAYER_WEST + SPRITE_ANIM_1
#define PLAYER_WEST_2  PLAYER_WEST + SPRITE_ANIM_2

// Items
#define ITEM_STICK PLAYER_WEST_2 + 1

// Projectiles
#define PROJECTILE_FIRE ITEM_STICK + 1

// Debug stuff
#define DEBUG_CIRCLE PROJECTILE_FIRE + 1

#define NUM_OF_SPRITES DEBUG_CIRCLE + 1

class Sprite
{
	private:
	  std::shared_ptr<Texture>                                   m_Texture;
	  static std::array<std::unique_ptr<Sprite>, NUM_OF_SPRITES> sprites;

	public:
	  Sprite(const char *texturePath);
	  ~Sprite();

	  void render(float x, float y, double rotation, float width, float height);

	  Texture *getTexture() { return m_Texture.get(); }

	  void bind(uint8_t slot = 0);
	  void unbind();

	  static Sprite *getSprite(int i) { return &*sprites[i]; };
	  static void    init()
	  {
		  // Tiles
		  sprites[BASIC_WALL]            = std::make_unique<Sprite>("res/textures/tiles/BasicWall.png");
		  sprites[BASIC_FLOOR]           = std::make_unique<Sprite>("res/textures/tiles/BasicFloor.png");
		  sprites[BASIC_OUTWARDS_CORNER] = std::make_unique<Sprite>("res/textures/tiles/BasicCorner.png");
		  sprites[BASIC_INWARDS_CORNER]  = std::make_unique<Sprite>("res/textures/tiles/BasicCorner2.png");

		  // Mobs
		  sprites[PLAYER_NORTH]   = std::make_unique<Sprite>("res/textures/entities/Player-heir-north.png");
		  sprites[PLAYER_NORTH_1] = std::make_unique<Sprite>("res/textures/entities/Player-heir-north-1.png");
		  sprites[PLAYER_NORTH_2] = std::make_unique<Sprite>("res/textures/entities/Player-heir-north-2.png");

		  sprites[PLAYER_SOUTH]   = std::make_unique<Sprite>("res/textures/entities/Player-heir-south.png");
		  sprites[PLAYER_SOUTH_1] = std::make_unique<Sprite>("res/textures/entities/Player-heir-south-1.png");
		  sprites[PLAYER_SOUTH_2] = std::make_unique<Sprite>("res/textures/entities/Player-heir-south-2.png");

		  sprites[PLAYER_EAST]   = std::make_unique<Sprite>("res/textures/entities/Player-heir-east.png");
		  sprites[PLAYER_EAST_1] = std::make_unique<Sprite>("res/textures/entities/Player-heir-east-1.png");
		  sprites[PLAYER_EAST_2] = std::make_unique<Sprite>("res/textures/entities/Player-heir-east-2.png");

		  sprites[PLAYER_WEST]   = std::make_unique<Sprite>("res/textures/entities/Player-heir-west.png");
		  sprites[PLAYER_WEST_1] = std::make_unique<Sprite>("res/textures/entities/Player-heir-west-1.png");
		  ;
		  sprites[PLAYER_WEST_2] = std::make_unique<Sprite>("res/textures/entities/Player-heir-west-2.png");
		  ;

		  // Stick
		  sprites[ITEM_STICK] = std::make_unique<Sprite>("res/textures/items/Stick.png");

		  // Projectiles
		  sprites[PROJECTILE_FIRE] = std::make_unique<Sprite>("res/textures/projectiles/FireProjectile.png");

		  // DEBUG STUFF
		  sprites[DEBUG_CIRCLE] = std::make_unique<Sprite>("res/textures/DebugCircle.png");

		  Log::info("Sprites have been loaded");
	}
};
