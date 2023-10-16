#pragma once

#include "2dVec.h"
#include "CollisionBox.h"

#include <algorithm>
#include <array>
#include <functional>
#include <vector>

#define X_MAX 2100
#define Y_MAX 2100

#define X_STEP 50
#define Y_STEP 50

// Node class which stores the parent and costs associated with it
struct Node
{
	Vec2i parent;
	float fCost, gCost, hCost;

	Node()
		: parent({-1, -1}), fCost(-1), gCost(-1), hCost(-1) {}
};

// Basic boolean operations to compare the cost of a node
inline bool operator<(const Node &lhs, const Node &rhs)
{
	return lhs.fCost < rhs.fCost;
}
inline bool operator>(const Node &lhs, const Node &rhs)
{
	return lhs.fCost > rhs.fCost;
}
inline bool operator==(const Node &lhs, const Node &rhs)
{
	return lhs.fCost == rhs.fCost;
}
inline bool operator!=(const Node &lhs, const Node &rhs)
{
	return lhs.fCost == rhs.fCost;
}

// NOTE: SORTING HIGHEST TO LOWEST
template <size_t ySize, size_t xSize>
inline int getIndexOfInsertion(std::vector<Vec2i> positions, std::array<std::array<Node, ySize>, xSize> &nodeMap, Vec2i nextPos)
{
	// This uses a modified binary search algorithm to find the position in which to place an element (as this is used in the A* it needed to be very efficient when dealing with large numbers)

	// If there is nothing in the list it returns the first element
	switch(positions.size())
	{
	case 0:
		return 0;
	case 1:
		// If the size is 1 it does a simple comparison
		if(nodeMap[positions[0].x][positions[0].y] > nodeMap[nextPos.x][nextPos.y])
			return 1;
		else
			return 0;
	default:   // If it is not one of these it will continue with the algorithm
		break;
	}

	// Sets the variables
	int startSub = 0;
	int endSub   = positions.size();
	int index    = positions.size() / 2;

	Node &node = nodeMap[nextPos.x][nextPos.y];

	// A simple lambda function to return the node of a given index of positions
	auto getNode = [&nodeMap, &positions](int index) -> Node & {
		return nodeMap[positions[index].x][positions[index].y];
	};

	// Runs a while loop until there range is too short
	while(startSub != endSub && startSub < endSub)
	{
		// If the index is out of points it changes so that it is in the bounds
		if(index + 1 >= positions.size())
			index = positions.size() - 2;

		// Gets the two nodes next to each other in the list to compare
		Node &thisNode = getNode(index);
		Node &nextNode = getNode(index + 1);

		// Compares the nodes to determine if it is found the position to return or if not how to change the borders
		if(thisNode == node || (thisNode > node && nextNode < node))
			return index + 1;
		else if(nextNode == node)
			return index + 2;
		else if(thisNode > node && nextNode > node)
			startSub = index + 2;
		else if(thisNode < node && nextNode < node)
			endSub = index;
		else
		{
			// If this is hit, it means the vector has not been sorted correctly so will cause an error
			Log::critical("Node vector is not sorted correctly!", LOGINFO);
			return -1;
		}

		index = (startSub + endSub) / 2;
	}

	// Returns the index if the range has been reduced
	return index;
}

// This is to clear up the log file by making sure that an error is only logged once and won't log anything until it has been fixed.
static bool hasWarned = false;

// NOTE: This is meant to deal with a number of situations in which to use A star, so if the grid is made to only be a portion of the map, the conversion must happen beforehand
template <size_t width, size_t height, size_t numOfPoints>
inline std::vector<Vec2f> *aStarAlgorithm(Vec2i startPos, Vec2i destPos, std::array<Vec2i, numOfPoints> &offsets, std::function<bool(int, int, int, int)> &collisionDetection, std::function<Vec2f(Vec2i)> &convert, int pathLimit)
{
	// Creates the vector that will be returned on the heap so that it does not get deleted when this function is exited
	std::vector<Vec2f> *path = new std::vector<Vec2f>();

	if(startPos.x != destPos.x || startPos.y != destPos.y)
	{
		// All of the next variables are allocated on the stack to improve efficiency as well to reduce the chance of memory leakage

		// Creates the openlist and reserves the maximum amount of nodes that will be on it (very unlikely to reach) this is so that it will speed up push_back and pop_back - however will not improve insert
		std::vector<Vec2i> openList;
		openList.reserve(width * height);

		// Creates the closed list and sets it all to false
		bool closedList[width][height];
		memset(closedList, false, sizeof(closedList));

		// Creates the node map - all the nodes will be created with the default constructor
		std::array<std::array<Node, height>, width> nodeMap;
		// Quick function that determines whether a node is valid

		auto skipThisPos = [&](Vec2i nextPos, Vec2i offset) -> bool {
			if(nextPos.x >= width || nextPos.x < 0 || nextPos.y >= height || nextPos.y < 0)
				return true;

			if(collisionDetection(nextPos.x - offset.x, nextPos.y - offset.y, offset.x, offset.y))
				return true;

			return closedList[nextPos.x][nextPos.y];
		};

		if(skipThisPos(destPos, {0, 0}))
		{
			// If a collision is detected at the destination the program won't work so it will log a warning and move on.
			skipThisPos(destPos, {0, 0});
			if(!hasWarned)
			{
				Log::error("A* Algorithm: Cannot reach destination!", LOGINFO);
				hasWarned = true;
			}

			// Just returns the destination in the path (this will make the mob just travel in the general direction)
			path->push_back(convert(destPos));

			return path;
		}

		{
			// Sets the start node and adds it to the open list
			float hCost                            = distBetweenVec(startPos, destPos);
			nodeMap[startPos.x][startPos.y].parent = {-1, -1};
			nodeMap[startPos.x][startPos.y].fCost  = hCost;
			nodeMap[startPos.x][startPos.y].gCost  = 0.0f;
			nodeMap[startPos.x][startPos.y].hCost  = hCost;

			openList.push_back(startPos);
		}

		while(openList.back() != destPos)
		{
			Vec2i currentPos  = openList.back();
			openList.pop_back();

			// Gets the current node for quick access later
			Node *currentNode = &nodeMap[currentPos.x][currentPos.y];
			closedList[currentPos.x][currentPos.y] = true;   // Sets the node to be closed

			// This goes through the list of possible offsets that has been given as a parameter
			for(Vec2i offset : offsets)
			{
				Vec2i nextPos = {currentPos.x + offset.x, currentPos.y + offset.y};

				if(skipThisPos(nextPos, offset))
					continue;

				// Calculates the cost of the node
				float gCost = currentNode->gCost + distBetweenVec(currentPos, nextPos);
				float hCost = distBetweenVec(nextPos, destPos);
				float fCost = gCost + hCost;

				// Checks if there is already a node in the node map
				if(nodeMap[nextPos.x][nextPos.y].fCost != -1)
				{
					// If the cost is greater than the cost of the node on the map it skips
					if(fCost >= nodeMap[nextPos.x][nextPos.y].fCost)
						continue;

					// Otherwise it updates the node in the map and removes it from the open list
					// We know it is in the open list because if it was closed it would have been picked up by skipThisPos funcition
					openList.erase(std::find(openList.begin(), openList.end(), nextPos));
				}

				nodeMap[nextPos.x][nextPos.y].parent = currentPos;
				nodeMap[nextPos.x][nextPos.y].fCost  = fCost;
				nodeMap[nextPos.x][nextPos.y].gCost  = gCost;
				nodeMap[nextPos.x][nextPos.y].hCost  = hCost;

				// Gets the index in which to insert the new node
				int insertIndex = getIndexOfInsertion(openList, nodeMap, nextPos);

				// Adds the node to the open list in the given postition
				openList.insert(openList.begin() + insertIndex, nextPos);
			}

			// Checks if the openList size is zero (if it is it means an error has happened)
			if(openList.size() == 0)
			{
				if(!hasWarned)
				{
					hasWarned = true;
					Log::warning("A* algorithm cannot continue as the open list has no elements in it!");
				}

				// Returns the destination node
				path->push_back(convert(destPos));
				return path;
			}
			else if(openList.size() > width * height)   // Checks if there is more nodes than there should be in the openList
			{
				// If there is something has gone wrong
				Log::critical("A* Algorithm: Too many nodes in the open list! Something is wrong!", LOGINFO);

				path->push_back(convert(destPos));
				return path;
			}
		}

		// If it has found a path it will continue follow the path backwards to get to the start position
		// This will store the current node it is looking at
		Vec2i currentPos = openList.back();
		while(currentPos != startPos)
		{
			Node *currentNode = &nodeMap[currentPos.x][currentPos.y];
			path->push_back(convert(currentPos));
			currentPos = currentNode->parent;

			if(path->size() > pathLimit)
			{
				if(!hasWarned)
				{
					Log::info("A* Algorithm: Path too long to destination, cancelling");
					hasWarned = true;
				}
				path->clear();
				path->push_back(convert(destPos));
				return path;
			}
		}
	}
	else
		path->push_back(convert(destPos));

	if(hasWarned)
		hasWarned = false;

	return path;
}