#pragma once

#include "Sprite.h"

// This contains one function that returns the path of a give sprite
inline std::string getPath(Sprite::ID id)
{
	std::string path;
	switch(id)
	{
	case Sprite::ID::tileBasicWall:
		path = "res/textures/tiles/Wall.png";
		break;
	case Sprite::ID::tileBasicFloor:
		path = "res/textures/tiles/Floor.png";
		break;
	case Sprite::ID::tileBasicExtCorner:
		path = "res/textures/tiles/ExternalCorner.png";
		break;
	case Sprite::ID::tileBasicIntCorner:
		path = "res/textures/tiles/InternalCorner.png";
		break;
	case Sprite::ID::tileBasicChest:
		path = "res/textures/tiles/Chest.png";
		break;
	case Sprite::ID::tileBasicTrapHidden:
		path = "res/textures/tiles/BasicTrap.png";
		break;
	case Sprite::ID::tileBasicTrapExposed:
		path = "res/textures/tiles/BasicTrapExposed.png";
		break;
	case Sprite::ID::tileBasicTrapdoor:
		path = "res/textures/tiles/Trapdoor.png";
		break;
	case Sprite::ID::mobPlayerNorth:
		path = "res/textures/entities/player/heir/North.png";
		break;
	case Sprite::ID::mobPlayerNorthWalk1:
		path = "res/textures/entities/player/heir/North-Walk-1.png";
		break;
	case Sprite::ID::mobPlayerNorthWalk2:
		path = "res/textures/entities/player/heir/North-Walk-2.png";
		break;
	case Sprite::ID::mobPlayerSouth:
		path = "res/textures/entities/player/heir/South.png";
		break;
	case Sprite::ID::mobPlayerSouthWalk1:
		path = "res/textures/entities/player/heir/South-Walk-1.png";
		break;
	case Sprite::ID::mobPlayerSouthWalk2:
		path = "res/textures/entities/player/heir/South-Walk-2.png";
		break;
	case Sprite::ID::mobPlayerEast:
		path = "res/textures/entities/player/heir/East.png";
		break;
	case Sprite::ID::mobPlayerEastWalk1:
		path = "res/textures/entities/player/heir/East-Walk-1.png";
		break;
	case Sprite::ID::mobPlayerEastWalk2:
		path = "res/textures/entities/player/heir/East-Walk-2.png";
		break;
	case Sprite::ID::mobPlayerWest:
		path = "res/textures/entities/player/heir/West.png";
		break;
	case Sprite::ID::mobPlayerWestWalk1:
		path = "res/textures/entities/player/heir/West-Walk-1.png";
		break;
	case Sprite::ID::mobPlayerWestWalk2:
		path = "res/textures/entities/player/heir/West-Walk-2.png";
		break;
	case Sprite::ID::followerFrostNorth:
		path = "res/textures/entities/followers/frost/North.png";
		break;
	case Sprite::ID::followerFrostNorthWalk1:
		path = "res/textures/entities/followers/frost/North-Walk-1.png";
		break;
	case Sprite::ID::followerFrostNorthWalk2:
		path = "res/textures/entities/followers/frost/North-Walk-2.png";
		break;
	case Sprite::ID::followerFrostSouth:
		path = "res/textures/entities/followers/frost/South.png";
		break;
	case Sprite::ID::followerFrostSouthWalk1:
		path = "res/textures/entities/followers/frost/South-Walk-1.png";
		break;
	case Sprite::ID::followerFrostSouthWalk2:
		path = "res/textures/entities/followers/frost/South-Walk-2.png";
		break;
	case Sprite::ID::followerFrostEast:
		path = "res/textures/entities/followers/frost/East.png";
		break;
	case Sprite::ID::followerFrostEastWalk1:
		path = "res/textures/entities/followers/frost/East-Walk-1.png";
		break;
	case Sprite::ID::followerFrostEastWalk2:
		path = "res/textures/entities/followers/frost/East-Walk-2.png";
		break;
	case Sprite::ID::followerFrostWest:
		path = "res/textures/entities/followers/frost/West.png";
		break;
	case Sprite::ID::followerFrostWestWalk1:
		path = "res/textures/entities/followers/frost/West-Walk-1.png";
		break;
	case Sprite::ID::followerFrostWestWalk2:
		path = "res/textures/entities/followers/frost/West-Walk-2.png";
		break;
	case Sprite::ID::followerFireNorth:
		path = "res/textures/entities/followers/fire/North.png";
		break;
	case Sprite::ID::followerFireNorthWalk1:
		path = "res/textures/entities/followers/fire/North-Walk-1.png";
		break;
	case Sprite::ID::followerFireNorthWalk2:
		path = "res/textures/entities/followers/fire/North-Walk-2.png";
		break;
	case Sprite::ID::followerFireSouth:
		path = "res/textures/entities/followers/fire/South.png";
		break;
	case Sprite::ID::followerFireSouthWalk1:
		path = "res/textures/entities/followers/fire/South-Walk-1.png";
		break;
	case Sprite::ID::followerFireSouthWalk2:
		path = "res/textures/entities/followers/fire/South-Walk-2.png";
		break;
	case Sprite::ID::followerFireEast:
		path = "res/textures/entities/followers/fire/East.png";
		break;
	case Sprite::ID::followerFireEastWalk1:
		path = "res/textures/entities/followers/fire/East-Walk-1.png";
		break;
	case Sprite::ID::followerFireEastWalk2:
		path = "res/textures/entities/followers/fire/East-Walk-2.png";
		break;
	case Sprite::ID::followerFireWest:
		path = "res/textures/entities/followers/fire/West.png";
		break;
	case Sprite::ID::followerFireWestWalk1:
		path = "res/textures/entities/followers/fire/West-Walk-1.png";
		break;
	case Sprite::ID::followerFireWestWalk2:
		path = "res/textures/entities/followers/fire/West-Walk-2.png";
		break;
	case Sprite::ID::followerDarkNorth:
		path = "res/textures/entities/followers/dark/North.png";
		break;
	case Sprite::ID::followerDarkNorthWalk1:
		path = "res/textures/entities/followers/dark/North-Walk-1.png";
		break;
	case Sprite::ID::followerDarkNorthWalk2:
		path = "res/textures/entities/followers/dark/North-Walk-2.png";
		break;
	case Sprite::ID::followerDarkSouth:
		path = "res/textures/entities/followers/dark/South.png";
		break;
	case Sprite::ID::followerDarkSouthWalk1:
		path = "res/textures/entities/followers/dark/South-Walk-1.png";
		break;
	case Sprite::ID::followerDarkSouthWalk2:
		path = "res/textures/entities/followers/dark/South-Walk-2.png";
		break;
	case Sprite::ID::followerDarkEast:
		path = "res/textures/entities/followers/dark/East.png";
		break;
	case Sprite::ID::followerDarkEastWalk1:
		path = "res/textures/entities/followers/dark/East-Walk-1.png";
		break;
	case Sprite::ID::followerDarkEastWalk2:
		path = "res/textures/entities/followers/dark/East-Walk-2.png";
		break;
	case Sprite::ID::followerDarkWest:
		path = "res/textures/entities/followers/dark/West.png";
		break;
	case Sprite::ID::followerDarkWestWalk1:
		path = "res/textures/entities/followers/dark/West-Walk-1.png";
		break;
	case Sprite::ID::followerDarkWestWalk2:
		path = "res/textures/entities/followers/dark/West-Walk-2.png";
		break;
	case Sprite::ID::enemyFrostNorth:
		path = "res/textures/entities/enemies/frost/North.png";
		break;
	case Sprite::ID::enemyFrostNorthWalk1:
		path = "res/textures/entities/enemies/frost/North-Walk-1.png";
		break;
	case Sprite::ID::enemyFrostNorthWalk2:
		path = "res/textures/entities/enemies/frost/North-Walk-2.png";
		break;
	case Sprite::ID::enemyFrostSouth:
		path = "res/textures/entities/enemies/frost/South.png";
		break;
	case Sprite::ID::enemyFrostSouthWalk1:
		path = "res/textures/entities/enemies/frost/South-Walk-1.png";
		break;
	case Sprite::ID::enemyFrostSouthWalk2:
		path = "res/textures/entities/enemies/frost/South-Walk-2.png";
		break;
	case Sprite::ID::enemyFrostEast:
		path = "res/textures/entities/enemies/frost/East.png";
		break;
	case Sprite::ID::enemyFrostEastWalk1:
		path = "res/textures/entities/enemies/frost/East-Walk-1.png";
		break;
	case Sprite::ID::enemyFrostEastWalk2:
		path = "res/textures/entities/enemies/frost/East-Walk-2.png";
		break;
	case Sprite::ID::enemyFrostWest:
		path = "res/textures/entities/enemies/frost/West.png";
		break;
	case Sprite::ID::enemyFrostWestWalk1:
		path = "res/textures/entities/enemies/frost/West-Walk-1.png";
		break;
	case Sprite::ID::enemyFrostWestWalk2:
		path = "res/textures/entities/enemies/frost/West-Walk-2.png";
		break;
	case Sprite::ID::enemyFireNorth:
		path = "res/textures/entities/enemies/fire/North.png";
		break;
	case Sprite::ID::enemyFireNorthWalk1:
		path = "res/textures/entities/enemies/fire/North-Walk-1.png";
		break;
	case Sprite::ID::enemyFireNorthWalk2:
		path = "res/textures/entities/enemies/fire/North-Walk-2.png";
		break;
	case Sprite::ID::enemyFireSouth:
		path = "res/textures/entities/enemies/fire/South.png";
		break;
	case Sprite::ID::enemyFireSouthWalk1:
		path = "res/textures/entities/enemies/fire/South-Walk-1.png";
		break;
	case Sprite::ID::enemyFireSouthWalk2:
		path = "res/textures/entities/enemies/fire/South-Walk-2.png";
		break;
	case Sprite::ID::enemyFireEast:
		path = "res/textures/entities/enemies/fire/East.png";
		break;
	case Sprite::ID::enemyFireEastWalk1:
		path = "res/textures/entities/enemies/fire/East-Walk-1.png";
		break;
	case Sprite::ID::enemyFireEastWalk2:
		path = "res/textures/entities/enemies/fire/East-Walk-2.png";
		break;
	case Sprite::ID::enemyFireWest:
		path = "res/textures/entities/enemies/fire/West.png";
		break;
	case Sprite::ID::enemyFireWestWalk1:
		path = "res/textures/entities/enemies/fire/West-Walk-1.png";
		break;
	case Sprite::ID::enemyFireWestWalk2:
		path = "res/textures/entities/enemies/fire/West-Walk-2.png";
		break;
	case Sprite::ID::enemyDarkNorth:
		path = "res/textures/entities/enemies/dark/North.png";
		break;
	case Sprite::ID::enemyDarkNorthWalk1:
		path = "res/textures/entities/enemies/dark/North-Walk-1.png";
		break;
	case Sprite::ID::enemyDarkNorthWalk2:
		path = "res/textures/entities/enemies/dark/North-Walk-2.png";
		break;
	case Sprite::ID::enemyDarkSouth:
		path = "res/textures/entities/enemies/dark/South.png";
		break;
	case Sprite::ID::enemyDarkSouthWalk1:
		path = "res/textures/entities/enemies/dark/South-Walk-1.png";
		break;
	case Sprite::ID::enemyDarkSouthWalk2:
		path = "res/textures/entities/enemies/dark/South-Walk-2.png";
		break;
	case Sprite::ID::enemyDarkEast:
		path = "res/textures/entities/enemies/dark/East.png";
		break;
	case Sprite::ID::enemyDarkEastWalk1:
		path = "res/textures/entities/enemies/dark/East-Walk-1.png";
		break;
	case Sprite::ID::enemyDarkEastWalk2:
		path = "res/textures/entities/enemies/dark/East-Walk-2.png";
		break;
	case Sprite::ID::enemyDarkWest:
		path = "res/textures/entities/enemies/dark/West.png";
		break;
	case Sprite::ID::enemyDarkWestWalk1:
		path = "res/textures/entities/enemies/dark/West-Walk-1.png";
		break;
	case Sprite::ID::enemyDarkWestWalk2:
		path = "res/textures/entities/enemies/dark/West-Walk-2.png";
		break;
	case Sprite::ID::itemStick:
		path = "res/textures/items/Stick.png";
		break;
	case Sprite::ID::weaponFireStaff:
		path = "res/textures/items/weapons/FireStaff.png";
		break;
	case Sprite::ID::weaponFrostStaff:
		path = "res/textures/items/weapons/FrostStaff.png";
		break;
	case Sprite::ID::weaponDarkStaff:
		path = "res/textures/items/weapons/DarkStaff.png";
		break;
	case Sprite::ID::weaponGoldStaff:
		path = "res/textures/items/weapons/GoldStaff.png";
		break;
	case Sprite::ID::weaponEarthStaff:
		path = "res/textures/items/weapons/EarthStaff.png";
		break;
	case Sprite::ID::weaponAirStaff:
		path = "res/textures/items/weapons/AirStaff.png";
		break;
	case Sprite::ID::weaponSling:
		path = "res/textures/items/weapons/Sling.png";
		break;
	case Sprite::ID::weaponBow:
		path = "res/textures/items/weapons/Bow.png";
		break;
	case Sprite::ID::weaponCrossbow:
		path = "res/textures/items/weapons/Crossbow.png";
		break;
	case Sprite::ID::weaponBoomerang:
		path = "res/textures/items/weapons/Boomerang.png";
		break;
	case Sprite::ID::bombRed:
		path = "res/textures/items/weapons/RedBomb.png";
		break;
	case Sprite::ID::bombPink:
		path = "res/textures/items/weapons/PinkBomb.png";
		break;
	case Sprite::ID::bombOrange:
		path = "res/textures/items/weapons/OrangeBomb.png";
		break;
	case Sprite::ID::potionRed:
		path = "res/textures/items/potions/bottles/Red.png";
		break;
	case Sprite::ID::potionRedRegen:
		path = "res/textures/items/potions/bottles/RedRegen.png";
		break;
	case Sprite::ID::potionRedMagic:
		path = "res/textures/items/potions/bottles/RedMagic.png";
		break;
	case Sprite::ID::potionRedHuge:
		path = "res/textures/items/potions/bottles/RedHuge.png";
		break;
	case Sprite::ID::potionBlue:
		path = "res/textures/items/potions/bottles/Blue.png";
		break;
	case Sprite::ID::potionBlueRegen:
		path = "res/textures/items/potions/bottles/BlueRegen.png";
		break;
	case Sprite::ID::potionBlueMagic:
		path = "res/textures/items/potions/bottles/BlueMagic.png";
		break;
	case Sprite::ID::potionBlueHuge:
		path = "res/textures/items/potions/bottles/BlueHuge.png";
		break;
	case Sprite::ID::potionGreen:
		path = "res/textures/items/potions/bottles/Green.png";
		break;
	case Sprite::ID::potionGreenRegen:
		path = "res/textures/items/potions/bottles/GreenRegen.png";
		break;
	case Sprite::ID::potionGreenMagic:
		path = "res/textures/items/potions/bottles/GreenMagic.png";
		break;
	case Sprite::ID::potionGreenHuge:
		path = "res/textures/items/potions/bottles/GreenHuge.png";
		break;
	case Sprite::ID::potionYellow:
		path = "res/textures/items/potions/bottles/Yellow.png";
		break;
	case Sprite::ID::potionYellowRegen:
		path = "res/textures/items/potions/bottles/YellowRegen.png";
		break;
	case Sprite::ID::potionYellowMagic:
		path = "res/textures/items/potions/bottles/YellowMagic.png";
		break;
	case Sprite::ID::potionYellowHuge:
		path = "res/textures/items/potions/bottles/YellowHuge.png";
		break;
	case Sprite::ID::books1:
		path = "res/textures/items/potions/books/Blue.png";
		break;
	case Sprite::ID::books2:
		path = "res/textures/items/potions/books/DarkBrown.png";
		break;
	case Sprite::ID::books3:
		path = "res/textures/items/potions/books/Green.png";
		break;
	case Sprite::ID::books4:
		path = "res/textures/items/potions/books/LightBrown.png";
		break;
	case Sprite::ID::books5:
		path = "res/textures/items/potions/books/Red.png";
		break;
	case Sprite::ID::books6:
		path = "res/textures/items/potions/books/Yellow.png";
		break;
	case Sprite::ID::magicBooks1:
		path = "res/textures/items/potions/books/MagicYellow.png";
		break;
	case Sprite::ID::magicBooks2:
		path = "res/textures/items/potions/books/MagicRed.png";
		break;
	case Sprite::ID::food1:
		path = "res/textures/items/potions/food/Apple.png";
		break;
	case Sprite::ID::food2:
		path = "res/textures/items/potions/food/Baguette.png";
		break;
	case Sprite::ID::food3:
		path = "res/textures/items/potions/food/Banana.png";
		break;
	case Sprite::ID::food4:
		path = "res/textures/items/potions/food/Boar.png";
		break;
	case Sprite::ID::food5:
		path = "res/textures/items/potions/food/Bread.png";
		break;
	case Sprite::ID::food6:
		path = "res/textures/items/potions/food/Carrot.png";
		break;
	case Sprite::ID::food7:
		path = "res/textures/items/potions/food/Chicken.png";
		break;
	case Sprite::ID::food8:
		path = "res/textures/items/potions/food/ChickenLeg.png";
		break;
	case Sprite::ID::food9:
		path = "res/textures/items/potions/food/Fish.png";
		break;
	case Sprite::ID::food10:
		path = "res/textures/items/potions/food/Grapes.png";
		break;
	case Sprite::ID::food11:
		path = "res/textures/items/potions/food/Lemon.png";
		break;
	case Sprite::ID::food12:
		path = "res/textures/items/potions/food/Meat.png";
		break;
	case Sprite::ID::food13:
		path = "res/textures/items/potions/food/Pear.png";
		break;
	case Sprite::ID::food14:
		path = "res/textures/items/potions/food/Strawberry.png";
		break;
	case Sprite::ID::food15:
		path = "res/textures/items/potions/food/Tomato.png";
		break;
	case Sprite::ID::projectileFire:
		path = "res/textures/projectiles/Fire.png";
		break;
	case Sprite::ID::projectileFrost:
		path = "res/textures/projectiles/Frost.png";
		break;
	case Sprite::ID::projectileDark:
		path = "res/textures/projectiles/Dark.png";
		break;
	case Sprite::ID::projectileGold:
		path = "res/textures/projectiles/Gold.png";
		break;
	case Sprite::ID::projectileNature:
		path = "res/textures/projectiles/Nature.png";
		break;
	case Sprite::ID::projectileRock:
		path = "res/textures/projectiles/Rock.png";
		break;
	case Sprite::ID::projectileArrow:
		path = "res/textures/projectiles/Arrow.png";
		break;
	case Sprite::ID::debugCircle:
		path = "res/textures/DebugCircle.png";
		break;
	case Sprite::ID::menuTitle:
		path = "res/menus/Title.png";
		break;
	default:
		Log::warning("Unknown sprite!");
		path = "res/textures/DebugCircle.png";
		break;
	}
	return path;
}