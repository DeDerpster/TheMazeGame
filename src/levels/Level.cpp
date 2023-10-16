#include "Level.h"

#include <array>

#include "Tile.h"

#define X_MAX 1400
#define Y_MAX 1400   // This is from the edge of the maze to the middle

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

bool Level::directionalCollision(float x, float y, float xs, float ys, CollisionBox collisionBox)
{
	if(x + xs < 0 || y + ys < 0 || x + xs > width * Tile::TILE_SIZE || y + ys > height * Tile::TILE_SIZE)
		return true;

	if(xs == 0 || ys == 0)
	{
		return collisionDetection(x + xs, y + ys, collisionBox);
	}
	else
	{
		return collisionDetection(x + xs / 2, y + ys / 2, collisionBox) || collisionDetection(x + xs, y + ys, collisionBox);
	}
}

std::vector<Vec2f> Level::getPath(Vec2f start, Vec2f destination, CollisionBox collisionBox)
{
	// Log::info("Generating path");
	// TODO: Add checks to see if beyond range
	start.x       = (int) (start.x / X_STEP) * X_STEP;
	start.y       = (int) (start.y / Y_STEP) * Y_STEP;
	destination.x = (int) (destination.x / X_STEP) * X_STEP;
	destination.y = (int) (destination.y / Y_STEP) * Y_STEP;
	std::vector<Vec2f> path;
	if(collisionDetection(destination.x, destination.y, collisionBox) /*|| collisionDetection(start.x, start.y)*/)
	{
		path.push_back(destination);
		return path;
	}
	if(start == destination)
	{
		Log::warning("Start is equal to destination, return destination");
		path.push_back(destination);
		return path;
	}
	std::vector<Node *> openList;   // This will be sorted most cost -> least cost
	std::vector<Node *> closedList;
	closedList.reserve((X_MAX / X_STEP) * (Y_MAX / Y_STEP));
	openList.reserve((X_MAX / X_STEP) * (Y_MAX / Y_STEP));

	Node *startNode = new Node(start, nullptr, 0, distBetweenVec2f(start, destination));

	openList.push_back(startNode);

	while(openList.back()->vec != destination)
	{
		Node *currentNode = openList.back();
		openList.pop_back();
		closedList.push_back(currentNode);

		for(int xs = -1; xs < 2; xs++)
		{
			for(int ys = -1; ys < 2; ys++)
			{
				float nextX = currentNode->vec.x + xs * X_STEP;
				float nextY = currentNode->vec.y + ys * Y_STEP;
				if((nextX - destination.x > X_MAX || nextX - destination.x < -X_MAX) && (nextY - destination.y > Y_MAX || nextY - destination.y < -Y_MAX))
				{
					Log::warning("Out of bounds following!");
					path.push_back(destination);
					for(Node *node : closedList)
						delete node;
					for(Node *node : openList)
						delete node;
					return path;
				}

				// if(collisionDetection(nextX, nextY, collisionBox) || collisionDetection(nextX, currentNode->vec.y, collisionBox) || collisionDetection(currentNode->vec.x, nextY, collisionBox))
				if(directionalCollision(currentNode->vec.x, currentNode->vec.y, xs * X_STEP, ys * Y_STEP, collisionBox))
					continue;

				Vec2f currentVec = {nextX, nextY};
				Node *nextNode   = new Node(currentVec, currentNode, currentNode->gCost + distBetweenVec2f(currentNode->vec, currentVec), distBetweenVec2f(currentVec, destination));

				if(nextNode->fCost > X_MAX + Y_MAX)
				{
					delete nextNode;
					continue;
				}
				if(nodeSearch(closedList, *nextNode) != -1)
				{
					delete nextNode;
					continue;
				}
				int index = nodeSearch(openList, *nextNode);
				if(index != -1)
				{
					if(nextNode->fCost >= openList[index]->fCost)
					{
						delete nextNode;
						continue;
					}
					delete openList[index];
					openList.erase(openList.begin() + index);
				}

				int insertIndex = getIndexOfInsertion(openList, *nextNode);
				if(insertIndex != openList.size() && ((openList.size() > 0 && *openList[insertIndex] > *nextNode) || (openList.size() > 1 && insertIndex != 0 && *openList[insertIndex - 1] < *nextNode)))
					Log::critical("Given wrong index!", LOGINFO);
				openList.insert(openList.begin() + insertIndex, nextNode);
			}
		}

		if(openList.size() == 0)
		{
			Log::warning("Cannot find route to destination");
			path.push_back(destination);
			for(Node *node : closedList)
				delete node;
			for(Node *node : openList)
				delete node;
			return path;
		}
	}

	Node *currentNode = openList.back();
	while(currentNode->vec != start)
	{
		path.push_back(currentNode->vec);
		currentNode = currentNode->parent;
	}

	for(Node *node : closedList)
		delete node;
	for(Node *node : openList)
		delete node;

	// Log::info("Finished generating");

	return path;
}