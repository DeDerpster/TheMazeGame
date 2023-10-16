#include "Level.h"

#include <algorithm>
#include <array>
#include <memory>

#include "AStarUtils.h"
#include "Entity.h"
#include "Player.h"
#include "Projectile.h"
#include "Tile.h"

Level::Level()
	: Layer(), width(0), height(0)
{
}

Level::Level(int width, int height)
	: Layer(), width(width), height(height)
{
}

Level::~Level()
{
	for(Entity *entity : m_Entities)
		delete entity;

	for(Projectile *projectile : m_Projectiles)
		delete projectile;
}

bool Level::collisionPointDetection(float nextX, float nextY)
{
	int   tileX = (int) nextX / Tile::TILE_SIZE;
	int   tileY = (int) nextY / Tile::TILE_SIZE;
	Tile *tile  = getTile(tileX, tileY);
	if(!tile)
		return true;

	return tile->isSolid();
}

bool Level::collisionTileDetection(int x, int y)
{
	Tile *tile = getTile(x, y);
	if(!tile)
		return true;

	return tile->isSolid();
}

bool Level::collisionDetection(float nextX, float nextY, CollisionBox box)
{
	if(nextX + box.lowerBound.x < 0 || nextY + box.lowerBound.y < 0 || nextX + box.upperBound.x > width * Tile::TILE_SIZE || nextY + box.upperBound.y > height * Tile::TILE_SIZE)
		return true;

	bool lowerLeft  = collisionPointDetection(nextX + box.lowerBound.x, nextY + box.lowerBound.y);
	bool lowerRight = collisionPointDetection(nextX + box.upperBound.x, nextY + box.lowerBound.y);
	bool upperLeft  = collisionPointDetection(nextX + box.lowerBound.x, nextY + box.upperBound.y);
	bool upperRight = collisionPointDetection(nextX + box.upperBound.x, nextY + box.upperBound.y);

	return lowerLeft || lowerRight || upperLeft || upperRight;
}

Entity *Level::entityCollisionDetection(float nextX, float nextY, CollisionBox box)
{
	for(Entity *e : m_Entities)
	{
		if(e->hasCollidedWith(nextX, nextY, box))
			return e;
	}
	return nullptr;
}

bool Level::lineCollisionDetection(float x, float y, float xs, float ys)
{
	bool noStep = collisionPointDetection(x, y);
	if(noStep)
		return true;

	// this is a simple function to return what sign a given float has
	// This is used so as a simple way to determine the direction of travel
	auto getSign = [](float check) -> int {
		if(check > 0)
			return 1;
		else
			return -1;
	};

	auto getPer = [](float relPos, float change) {
		// This calculates the percentage of the way through the tile boundary is hit, and thus the first boundary to be hit
		if(change > 0)
		{
			if(relPos + change > Tile::TILE_SIZE)
				return (Tile::TILE_SIZE - relPos) / change;
		}
		else
		{   // This means change is negative so it has to check it doesn't go passed 0
			if(relPos + change < 0.0f)
				return relPos / change;
		}
		// Set to 1.1 so that if it doesn't hit another boundary it does not interfere with the comparason
		return 1.1f;
	};

	// This makes the coordinates relative to the current the start of the line is on, making it easier for calculations later
	int   tileX     = (int) x / Tile::TILE_SIZE;
	int   tileY     = (int) y / Tile::TILE_SIZE;
	float relativeX = x - tileX * Tile::TILE_SIZE;
	float relativeY = y - tileY * Tile::TILE_SIZE;

	if(relativeX + xs < Tile::TILE_SIZE && relativeX + xs > 0 && relativeY + ys < Tile::TILE_SIZE && relativeY + ys > 0)
		return noStep;

	int xOffset = 0;
	int yOffset = 0;
	// This continues running until the xs or ys does not travel over a tile boundary
	while(relativeX + xs > Tile::TILE_SIZE || relativeY + ys > Tile::TILE_SIZE || relativeX + xs < 0 || relativeY + ys < 0)
	{
		float xPer = getPer(relativeX, xs);
		float yPer = getPer(relativeY, ys);

		if(xPer < yPer)   // This just goes through and checks with tile to check
		{
			xOffset += getSign(xs);
			if(collisionTileDetection(tileX + xOffset, tileY + yOffset))
				return true;
			relativeX -= getSign(xs) * Tile::TILE_SIZE;
		}
		else if(yPer < xPer)
		{
			yOffset += getSign(ys);
			if(collisionTileDetection(tileX + xOffset, tileY + yOffset))
				return true;
			relativeY -= getSign(ys) * Tile::TILE_SIZE;
		}
		else   // This means that it passes through the corner
		{
			if(collisionTileDetection(tileX + xOffset + getSign(xs), tileY + yOffset) || collisionTileDetection(tileX + xOffset, tileY + yOffset + getSign(ys)) || collisionTileDetection(tileX + xOffset + getSign(xs), tileY + xOffset + getSign(ys)))
				return true;

			relativeX -= getSign(xs) * Tile::TILE_SIZE;
			relativeY -= getSign(ys) * Tile::TILE_SIZE;

			xOffset += getSign(xs);
			yOffset += getSign(ys);
		}
	}

	return false;
}

bool Level::directionalCollision(float x, float y, float xs, float ys, CollisionBox box)
{
	if(xs == 0 && ys == 0)
		return collisionDetection(x, y, box);
	else
	{
		if(x + box.lowerBound.x < 0 || y + box.lowerBound.y < 0 || x + box.upperBound.x > width * Tile::TILE_SIZE || y + box.upperBound.y > height * Tile::TILE_SIZE)
			return true;

		if((xs > 0 && ys > 0) || (xs < 0 && ys < 0))   // Travelling along a line close to this: / path
		{
			bool lowerRight = lineCollisionDetection(x + box.upperBound.x, y + box.lowerBound.y, xs, ys);
			bool upperLeft  = lineCollisionDetection(x + box.lowerBound.x, y + box.upperBound.y, xs, ys);

			bool middle;
			if(xs > 0)   // Upper right corner
				middle = lineCollisionDetection(x + box.upperBound.x, y + box.upperBound.y, xs, ys);
			else   // Lower left corner
				middle = lineCollisionDetection(x + box.lowerBound.x, y + box.lowerBound.y, xs, ys);

			// bool middle     = lineCollisionDetection(x + box.lowerBound.x, y + box.lowerBound.y, xs + box.upperBound.x - box.lowerBound.x, ys + box.upperBound.y - box.lowerBound.y);
			return lowerRight || upperLeft || middle;
		}
		else if((xs > 0 && ys < 0) || (xs < 0 && ys > 0))   // Travelling along a line close to this: \ path
		{
			bool lowerLeft  = lineCollisionDetection(x + box.lowerBound.x, y + box.lowerBound.y, xs, ys);
			bool upperRight = lineCollisionDetection(x + box.upperBound.x, y + box.upperBound.y, xs, ys);

			bool middle;
			if(xs > 0)   // Lower right corner
				middle = lineCollisionDetection(x + box.upperBound.x, y + box.lowerBound.y, xs, ys);
			else   // Upper left corner
				middle = lineCollisionDetection(x + box.lowerBound.x, y + box.upperBound.y, xs, ys);

			// bool middle     = lineCollisionDetection(x + box.upperBound.x, y + box.lowerBound.y, xs + box.lowerBound.x - box.upperBound.x, ys + box.upperBound.y - box.lowerBound.y);
			return lowerLeft || upperRight || middle;
		}
		else if(xs != 0)   // Travelling along a line perpendicular to ---
		{
			bool upper, lower;
			if(xs > 0)   // Check right side
			{
				upper = lineCollisionDetection(x + box.upperBound.x, y + box.upperBound.y, xs, ys);
				lower = lineCollisionDetection(x + box.upperBound.x, y + box.lowerBound.y, xs, ys);
			}
			else   // Check left side
			{
				upper = lineCollisionDetection(x + box.lowerBound.x, y + box.upperBound.y, xs, ys);
				lower = lineCollisionDetection(x + box.lowerBound.x, y + box.lowerBound.y, xs, ys);
			}
			return upper || lower;
		}
		else   // Travelling along a line perpendicular to |
		{
			bool left, right;
			if(ys > 0)   // Check upper side
			{
				right = lineCollisionDetection(x + box.upperBound.x, y + box.upperBound.y, xs, ys);
				left  = lineCollisionDetection(x + box.lowerBound.x, y + box.upperBound.y, xs, ys);
			}
			else   // Check lower side
			{
				right = lineCollisionDetection(x + box.upperBound.x, y + box.lowerBound.y, xs, ys);
				left  = lineCollisionDetection(x + box.lowerBound.x, y + box.lowerBound.y, xs, ys);
			}
			return left || right;
		}
	}
}

std::vector<Vec2f> *Level::getPath(Vec2f startPos, Vec2f dest, CollisionBox box)
{
	// FIXME
	Vec2i start       = {(int) round(startPos.x / X_STEP), (int) round(startPos.y / Y_STEP)};
	Vec2i destination = {(int) round(dest.x / X_STEP), (int) round(dest.y / Y_STEP)};

	std::vector<Vec2f> *path = new std::vector<Vec2f>();
	if(collisionDetection(destination.x * X_STEP, destination.y * Y_STEP, box))
	{
		bool foundAlternative = false;

		for(int x = -1; x < 2; x++)
		{
			for(int y = -1; y < 2; y++)
			{
				if(x == 0 && y == 0)
					continue;
				if(!collisionDetection((destination.x + x) * X_STEP, (destination.y + y) * Y_STEP, box))
				{
					destination.x += x;
					destination.y += y;
					foundAlternative = true;
					break;
				}
			}
		}

		if(!foundAlternative)
		{
			Log::warning("Cannot reach destination!");
			path->push_back(dest);
			return path;
		}
	}
	if(start == destination)
	{
		path->push_back(dest);
		return path;
	}

	Vec2i relativeStart       = {X_MAX / X_STEP, Y_MAX / Y_STEP};
	Vec2i relativeDestination = {destination.x - start.x + relativeStart.x, destination.y - start.y + relativeStart.y};

	std::array<Vec2i, 8> offsets;
	offsets[0] = {-1, 1};
	offsets[1] = {0, 1};
	offsets[2] = {1, 1};
	offsets[3] = {1, 0};
	offsets[4] = {1, -1};
	offsets[5] = {0, -1};
	offsets[6] = {-1, -1};
	offsets[7] = {-1, 0};

	std::function<Vec2f(Vec2i)> convert = [relativeStart, start](Vec2i vec) -> Vec2f {
		return {(float) (vec.x - relativeStart.x + start.x) * X_STEP, (float) (vec.y - relativeStart.y + start.y) * Y_STEP};
	};

	std::function<bool(int, int, int, int, CollisionBox)> collisionDetection = [this, &convert](int x, int y, int xs, int ys, CollisionBox box) -> bool {
		Vec2f pos = convert({x, y});
		return directionalCollision(pos.x, pos.y, xs * X_STEP, ys * Y_STEP, box);
	};

	Vec2f checkDest = {(float) destination.x * X_STEP, (float) destination.y * Y_STEP};

	if(convert(relativeDestination) != checkDest)
		Log::critical("Convertion does not result in the correct value!", LOGINFO);

	// Log::info("Level A*");

	return aStarAlgorithm<2 * X_MAX / X_STEP, 2 * Y_MAX / Y_STEP, 8>(relativeStart, relativeDestination, box, offsets, collisionDetection, convert, X_MAX / X_STEP);
}
