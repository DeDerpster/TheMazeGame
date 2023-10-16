#pragma once
#include <array>
#include <memory>
#include <string>

#include "Log.h"
#include "Texture.h"
#include "Utils.h"

#define SPRITE_WALK_1 1
#define SPRITE_WALK_2 2

#define SPRITE_NORTH 0
#define SPRITE_SOUTH 3
#define SPRITE_EAST  6
#define SPRITE_WEST  9
#define SPRITE_END   12

#define SPRITE_FROST 0 * SPRITE_END
#define SPRITE_FIRE  1 * SPRITE_END
#define SPRITE_DARK  2 * SPRITE_END

#define POTION_SPRITES 4
#define POTION_REGEN   1
#define POTION_MAGIC   2
#define POTION_HUGE    3

#define SPRITE_BOOK_NUM 6
#define SPRITE_MAGIC_BOOK_NUM 2
#define SPRITE_FOOD_NUM 15

class Sprite
{
  public:
	enum ID
	{
		errorID = -1,

		tileBasicWall,
		tileBasicFloor,
		tileBasicExtCorner,
		tileBasicIntCorner,
		tileBasicChest,
		tileBasicTrapHidden,
		tileBasicTrapExposed,
		tileBasicTrapdoor,

		mobPlayer,
		mobPlayerNorth      = mobPlayer + SPRITE_NORTH,
		mobPlayerNorthWalk1 = mobPlayer + SPRITE_NORTH + SPRITE_WALK_1,
		mobPlayerNorthWalk2 = mobPlayer + SPRITE_NORTH + SPRITE_WALK_2,
		mobPlayerSouth      = mobPlayer + SPRITE_SOUTH,
		mobPlayerSouthWalk1 = mobPlayer + SPRITE_SOUTH + SPRITE_WALK_1,
		mobPlayerSouthWalk2 = mobPlayer + SPRITE_SOUTH + SPRITE_WALK_2,
		mobPlayerEast       = mobPlayer + SPRITE_EAST,
		mobPlayerEastWalk1  = mobPlayer + SPRITE_EAST + SPRITE_WALK_1,
		mobPlayerEastWalk2  = mobPlayer + SPRITE_EAST + SPRITE_WALK_2,
		mobPlayerWest       = mobPlayer + SPRITE_WEST,
		mobPlayerWestWalk1  = mobPlayer + SPRITE_WEST + SPRITE_WALK_1,
		mobPlayerWestWalk2  = mobPlayer + SPRITE_WEST + SPRITE_WALK_2,

		followerFrost           = mobPlayer + SPRITE_END,
		followerFrostNorth      = followerFrost + SPRITE_NORTH,
		followerFrostNorthWalk1 = followerFrost + SPRITE_NORTH + SPRITE_WALK_1,
		followerFrostNorthWalk2 = followerFrost + SPRITE_NORTH + SPRITE_WALK_2,
		followerFrostSouth      = followerFrost + SPRITE_SOUTH,
		followerFrostSouthWalk1 = followerFrost + SPRITE_SOUTH + SPRITE_WALK_1,
		followerFrostSouthWalk2 = followerFrost + SPRITE_SOUTH + SPRITE_WALK_2,
		followerFrostEast       = followerFrost + SPRITE_EAST,
		followerFrostEastWalk1  = followerFrost + SPRITE_EAST + SPRITE_WALK_1,
		followerFrostEastWalk2  = followerFrost + SPRITE_EAST + SPRITE_WALK_2,
		followerFrostWest       = followerFrost + SPRITE_WEST,
		followerFrostWestWalk1  = followerFrost + SPRITE_WEST + SPRITE_WALK_1,
		followerFrostWestWalk2  = followerFrost + SPRITE_WEST + SPRITE_WALK_2,

		followerFire           = followerFrost + SPRITE_END,
		followerFireNorth      = followerFire + SPRITE_NORTH,
		followerFireNorthWalk1 = followerFire + SPRITE_NORTH + SPRITE_WALK_1,
		followerFireNorthWalk2 = followerFire + SPRITE_NORTH + SPRITE_WALK_2,
		followerFireSouth      = followerFire + SPRITE_SOUTH,
		followerFireSouthWalk1 = followerFire + SPRITE_SOUTH + SPRITE_WALK_1,
		followerFireSouthWalk2 = followerFire + SPRITE_SOUTH + SPRITE_WALK_2,
		followerFireEast       = followerFire + SPRITE_EAST,
		followerFireEastWalk1  = followerFire + SPRITE_EAST + SPRITE_WALK_1,
		followerFireEastWalk2  = followerFire + SPRITE_EAST + SPRITE_WALK_2,
		followerFireWest       = followerFire + SPRITE_WEST,
		followerFireWestWalk1  = followerFire + SPRITE_WEST + SPRITE_WALK_1,
		followerFireWestWalk2  = followerFire + SPRITE_WEST + SPRITE_WALK_2,

		followerDark           = followerFire + SPRITE_END,
		followerDarkNorth      = followerDark + SPRITE_NORTH,
		followerDarkNorthWalk1 = followerDark + SPRITE_NORTH + SPRITE_WALK_1,
		followerDarkNorthWalk2 = followerDark + SPRITE_NORTH + SPRITE_WALK_2,
		followerDarkSouth      = followerDark + SPRITE_SOUTH,
		followerDarkSouthWalk1 = followerDark + SPRITE_SOUTH + SPRITE_WALK_1,
		followerDarkSouthWalk2 = followerDark + SPRITE_SOUTH + SPRITE_WALK_2,
		followerDarkEast       = followerDark + SPRITE_EAST,
		followerDarkEastWalk1  = followerDark + SPRITE_EAST + SPRITE_WALK_1,
		followerDarkEastWalk2  = followerDark + SPRITE_EAST + SPRITE_WALK_2,
		followerDarkWest       = followerDark + SPRITE_WEST,
		followerDarkWestWalk1  = followerDark + SPRITE_WEST + SPRITE_WALK_1,
		followerDarkWestWalk2  = followerDark + SPRITE_WEST + SPRITE_WALK_2,

		enemyFrost           = followerDark + SPRITE_END,
		enemyFrostNorth      = enemyFrost + SPRITE_NORTH,
		enemyFrostNorthWalk1 = enemyFrost + SPRITE_NORTH + SPRITE_WALK_1,
		enemyFrostNorthWalk2 = enemyFrost + SPRITE_NORTH + SPRITE_WALK_2,
		enemyFrostSouth      = enemyFrost + SPRITE_SOUTH,
		enemyFrostSouthWalk1 = enemyFrost + SPRITE_SOUTH + SPRITE_WALK_1,
		enemyFrostSouthWalk2 = enemyFrost + SPRITE_SOUTH + SPRITE_WALK_2,
		enemyFrostEast       = enemyFrost + SPRITE_EAST,
		enemyFrostEastWalk1  = enemyFrost + SPRITE_EAST + SPRITE_WALK_1,
		enemyFrostEastWalk2  = enemyFrost + SPRITE_EAST + SPRITE_WALK_2,
		enemyFrostWest       = enemyFrost + SPRITE_WEST,
		enemyFrostWestWalk1  = enemyFrost + SPRITE_WEST + SPRITE_WALK_1,
		enemyFrostWestWalk2  = enemyFrost + SPRITE_WEST + SPRITE_WALK_2,

		enemyFire           = enemyFrost + SPRITE_END,
		enemyFireNorth      = enemyFire + SPRITE_NORTH,
		enemyFireNorthWalk1 = enemyFire + SPRITE_NORTH + SPRITE_WALK_1,
		enemyFireNorthWalk2 = enemyFire + SPRITE_NORTH + SPRITE_WALK_2,
		enemyFireSouth      = enemyFire + SPRITE_SOUTH,
		enemyFireSouthWalk1 = enemyFire + SPRITE_SOUTH + SPRITE_WALK_1,
		enemyFireSouthWalk2 = enemyFire + SPRITE_SOUTH + SPRITE_WALK_2,
		enemyFireEast       = enemyFire + SPRITE_EAST,
		enemyFireEastWalk1  = enemyFire + SPRITE_EAST + SPRITE_WALK_1,
		enemyFireEastWalk2  = enemyFire + SPRITE_EAST + SPRITE_WALK_2,
		enemyFireWest       = enemyFire + SPRITE_WEST,
		enemyFireWestWalk1  = enemyFire + SPRITE_WEST + SPRITE_WALK_1,
		enemyFireWestWalk2  = enemyFire + SPRITE_WEST + SPRITE_WALK_2,

		enemyDark           = enemyFire + SPRITE_END,
		enemyDarkNorth      = enemyDark + SPRITE_NORTH,
		enemyDarkNorthWalk1 = enemyDark + SPRITE_NORTH + SPRITE_WALK_1,
		enemyDarkNorthWalk2 = enemyDark + SPRITE_NORTH + SPRITE_WALK_2,
		enemyDarkSouth      = enemyDark + SPRITE_SOUTH,
		enemyDarkSouthWalk1 = enemyDark + SPRITE_SOUTH + SPRITE_WALK_1,
		enemyDarkSouthWalk2 = enemyDark + SPRITE_SOUTH + SPRITE_WALK_2,
		enemyDarkEast       = enemyDark + SPRITE_EAST,
		enemyDarkEastWalk1  = enemyDark + SPRITE_EAST + SPRITE_WALK_1,
		enemyDarkEastWalk2  = enemyDark + SPRITE_EAST + SPRITE_WALK_2,
		enemyDarkWest       = enemyDark + SPRITE_WEST,
		enemyDarkWestWalk1  = enemyDark + SPRITE_WEST + SPRITE_WALK_1,
		enemyDarkWestWalk2  = enemyDark + SPRITE_WEST + SPRITE_WALK_2,

		itemStick,

		weaponFireStaff,
		weaponFrostStaff,
		weaponDarkStaff,
		weaponGoldStaff,
		weaponEarthStaff,
		weaponAirStaff,
		weaponSling,
		weaponBow,
		weaponCrossbow,
		weaponBoomerang,

		bombRed,
		bombPink,
		bombOrange,

		potionRed,
		potionRedRegen = potionRed + POTION_REGEN,
		potionRedMagic = potionRed + POTION_MAGIC,
		potionRedHuge  = potionRed + POTION_HUGE,

		potionBlue   = potionRed + POTION_SPRITES,
		potionBlueRegen = potionBlue + POTION_REGEN,
		potionBlueMagic = potionBlue + POTION_MAGIC,
		potionBlueHuge  = potionBlue + POTION_HUGE,

		potionGreen  = potionBlue + POTION_SPRITES,
		potionGreenRegen = potionGreen + POTION_REGEN,
		potionGreenMagic = potionGreen + POTION_MAGIC,
		potionGreenHuge  = potionGreen + POTION_HUGE,

		potionYellow = potionGreen + POTION_SPRITES,
		potionYellowRegen = potionYellow + POTION_REGEN,
		potionYellowMagic = potionYellow + POTION_MAGIC,
		potionYellowHuge  = potionYellow + POTION_HUGE,

		books1,
		books2,
		books3,
		books4,
		books5,
		books6,
		magicBooks1,
		magicBooks2,

		food1,
		food2,
		food3,
		food4,
		food5,
		food6,
		food7,
		food8,
		food9,
		food10,
		food11,
		food12,
		food13,
		food14,
		food15,

		projectileFire,
		projectileFrost,
		projectileDark,
		projectileGold,
		projectileNature,
		projectileRock,
		projectileArrow,

		debugCircle,

		menuTitle,

		numOfSprites,


	};

  public:
	Sprite(ID id);
	Sprite(const char *texturePath);
	~Sprite();

	void render(float x, float y, double rotation, float width, float height);

	Texture *getTexture() { return m_Texture.get(); }

	void bind(uint8_t slot = 0);
	void unbind();

	static Sprite *getSprite(ID i) { return &*sprites[static_cast<int>(i)]; };
	static void    init();

  private:
	std::shared_ptr<Texture>                                             m_Texture;
	static std::array<std::unique_ptr<Sprite>, Sprite::ID::numOfSprites> sprites;
};


inline Sprite::ID operator+(const Sprite::ID &m, const int &o)
{
	if(o < -1 || o > Sprite::ID::numOfSprites)
		Log::critical("Cannot turn into sprite!", LOGINFO);

	return static_cast<Sprite::ID>(static_cast<int>(m) + o);
}

inline Sprite::ID operator-(const Sprite::ID &m, const int &o)
{
	if(o < -1 || o > Sprite::ID::numOfSprites)
		Log::critical("Cannot turn into sprite!", LOGINFO);
	return static_cast<Sprite::ID>(static_cast<int>(m) - o);
}

inline Sprite::ID &operator+=(Sprite::ID &m, const int &o)
{
	m = m + static_cast<Sprite::ID>(o);
	return m;
}

inline Sprite::ID &operator-=(Sprite::ID &m, const int &o)
{
	m = m - static_cast<Sprite::ID>(o);
	return m;
}

inline Sprite::ID &operator++(Sprite::ID &m)
{
	m = m + static_cast<Sprite::ID>(1);
	return m;
}

#define SPRITE_BOOK_START       Sprite::ID::books1
#define SPRITE_MAGIC_BOOK_START Sprite::ID::magicBooks1
#define SPRITE_FOOD_START       Sprite::ID::food1
