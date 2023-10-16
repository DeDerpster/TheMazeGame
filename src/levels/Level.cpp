#include "Level.h"

#include <algorithm>
#include <array>
#include <memory>

#include "AStarUtils.h"
#include "Entity.h"
#include "Player.h"
#include "Projectile.h"
#include "Tile.h"

#define X_MAX 4500
#define Y_MAX 4500   // This is from the edge of the maze to the middle

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

bool Level::collisionDetection(float nextX, float nextY, CollisionBox collisionBox)
{
	if(nextX < 0 || nextY < 0 || nextX > width * Tile::TILE_SIZE || nextY > height * Tile::TILE_SIZE)
		return true;

	bool lowerLeft  = collisionPointDetection(nextX + collisionBox.lowerBound.x, nextY + collisionBox.lowerBound.y);
	bool lowerRight = collisionPointDetection(nextX + collisionBox.upperBound.x, nextY + collisionBox.lowerBound.y);
	bool upperLeft  = collisionPointDetection(nextX + collisionBox.lowerBound.x, nextY + collisionBox.upperBound.y);
	bool upperRight = collisionPointDetection(nextX + collisionBox.upperBound.x, nextY + collisionBox.upperBound.y);

	return lowerLeft || lowerRight || upperLeft || upperRight;
}

Entity *Level::entityCollisionDetection(float nextX, float nextY, CollisionBox collisionBox)
{
	for(Entity *e : m_Entities)
	{
		if(e->hasCollidedWith(nextX, nextY, collisionBox))
			return e;
	}
	return nullptr;
}

bool Level::directionalCollision(float x, float y, float xs, float ys, CollisionBox collisionBox)
{
	if(xs == 0 || ys == 0)
	{
		return collisionDetection(x + xs, y + ys, collisionBox);
	}
	else
	{
		return collisionDetection(x + xs / 2, y + ys / 2, collisionBox) || collisionDetection(x + xs, y + ys, collisionBox);
	}
}

std::vector<Vec2f> *Level::getPath(Vec2f startPos, Vec2f dest, CollisionBox collisionBox)
{
	// FIXME
	Vec2i start       = {(int) round(startPos.x / X_STEP), (int) round(startPos.y / Y_STEP)};
	Vec2i destination = {(int) round(dest.x / X_STEP), (int) round(dest.y / Y_STEP)};

	std::vector<Vec2f> *path = new std::vector<Vec2f>();
	if(collisionDetection(destination.x * X_STEP, destination.y * Y_STEP, collisionBox))
	{
		path->push_back(dest);
		return path;
	}
	if(start == destination)
	{
		Log::warning("Start is equal to destination, return destination");
		path->push_back(dest);
		return path;
	}

	std::vector<Vec2i> openList;   // This will be sorted most cost -> least cost

	bool closedList[(X_MAX * 2) / X_STEP][(Y_MAX * 2) / Y_STEP];
	memset(closedList, false, sizeof(closedList));

	std::unique_ptr<std::array<std::array<Node, (Y_MAX * 2 / Y_STEP)>, (X_MAX * 2 / X_STEP)>> nodeMap = std::make_unique<std::array<std::array<Node, (Y_MAX * 2 / Y_STEP)>, (X_MAX * 2 / X_STEP)>>();

	openList.reserve(4 * (X_MAX / X_STEP) + 4 * (Y_MAX / Y_STEP));

	Vec2i startVec            = {X_MAX / X_STEP, Y_MAX / Y_STEP};
	Vec2i relativeDestination = {destination.x - start.x + startVec.x, destination.y - start.y + startVec.y};
	{
		float hCost                               = distBetweenVec2i(start, destination);
		(*nodeMap)[startVec.x][startVec.y].vec    = start;
		(*nodeMap)[startVec.x][startVec.y].parent = {-1, -1};
		(*nodeMap)[startVec.x][startVec.y].fCost  = hCost;
		(*nodeMap)[startVec.x][startVec.y].gCost  = 0.0f;
		(*nodeMap)[startVec.x][startVec.y].hCost  = hCost;

		openList.push_back(startVec);
	}

	while(openList.back() != relativeDestination)
	{
		Vec2i currentPos  = openList.back();
		Node *currentNode = &(*nodeMap)[currentPos.x][currentPos.y];
		openList.pop_back();
		closedList[currentPos.x][currentPos.y] = true;

		for(int xs = -1; xs < 2; xs++)
		{
			for(int ys = -1; ys < 2; ys++)
			{
				if(xs == 0 && ys == 0) continue;
				Vec2i nextPos = {currentPos.x + xs, currentPos.y + ys};
				if(nextPos.x > nodeMap->size() || nextPos.x < 0 || nextPos.y > (*nodeMap)[0].size() || nextPos.y < 0)
					continue;

				if(directionalCollision(currentNode->vec.x * X_STEP, currentNode->vec.y * Y_STEP, xs * X_STEP, ys * Y_STEP, collisionBox))
					continue;

				Vec2i currentVec = {currentNode->vec.x + xs, currentNode->vec.y + ys};

				if(closedList[nextPos.x][nextPos.y])
					continue;

				float gCost = currentNode->gCost + distBetweenVec2i(currentNode->vec, currentVec);
				float hCost = distBetweenVec2i(currentVec, destination);
				float fCost = gCost + hCost;

				Node &nextNode = (*nodeMap)[nextPos.x][nextPos.y];
				if(nextNode.vec.x != -1)
				{
					if(fCost >= nextNode.fCost)
						continue;
					nextNode.parent = currentPos;
					nextNode.fCost  = fCost;
					nextNode.gCost  = gCost;
					nextNode.hCost  = hCost;
					openList.erase(std::remove(openList.begin(), openList.end(), nextPos), openList.end());
				}
				else
				{
					nextNode.vec    = currentVec;
					nextNode.parent = currentPos;
					nextNode.fCost  = fCost;
					nextNode.gCost  = gCost;
					nextNode.hCost  = hCost;
				}

				int insertIndex = getIndexOfInsertion(openList, (*nodeMap), nextPos);
				if(insertIndex == -1)
				{
					path->push_back(dest);
					return path;
				}

				openList.insert(openList.begin() + insertIndex, nextPos);
			}
		}

		if(openList.size() == 0)
		{
			Log::warning("Cannot find route to destination");
			path->push_back(dest);
			return path;
		}
	}

	Vec2i currentPos = openList.back();
	while(currentPos != startVec)
	{
		Node *currentNode = &(*nodeMap)[currentPos.x][currentPos.y];
		path->push_back({(float) currentNode->vec.x * X_STEP, (float) currentNode->vec.y * Y_STEP});
		currentPos = currentNode->parent;
		if(path->size() > X_MAX / X_STEP)
		{
			Log::info("Path too long, cancelling");
			path->clear();
			path->push_back(dest);
			return path;
		}
	}

	return path;
}
