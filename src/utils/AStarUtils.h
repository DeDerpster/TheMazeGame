#pragma once

#include <functional>
#include <vector>
#include <algorithm>
#include <array>

// #include <filesystem>
// #include <fstream>

#include "Utils.h"

#define X_MAX 2100
#define Y_MAX 2100   // This is from the edge of the maze to the middle

#define X_STEP 50
#define Y_STEP 50
struct Node
{
	Vec2i parent;
	float fCost, gCost, hCost;

	Node()
		: parent({-1, -1}), fCost(-1), gCost(-1), hCost(-1) {}
};

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

// SORTING HIGHEST TO LOWEST
template <size_t ySize, size_t xSize>
inline int getIndexOfInsertion(std::vector<Vec2i> positions, std::array<std::array<Node, ySize>, xSize> &nodeMap, Vec2i nextPos)
{
	if(positions.size() == 0)
		return 0;
	if(positions.size() == 1)
	{
		if(nodeMap[positions[0].x][positions[0].y] > nodeMap[nextPos.x][nextPos.y])
			return 1;
		else
			return 0;
	}
	int startSub = 0;
	int endSub   = positions.size();
	int index    = positions.size() / 2;

	Node &node = nodeMap[nextPos.x][nextPos.y];

	while(startSub != endSub && startSub < endSub)
	{
		if(index + 1 == positions.size())
			index--;

		Node &thisNode = nodeMap[positions[index].x][positions[index].y];
		Node &nextNode = nodeMap[positions[index + 1].x][positions[index + 1].y];

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
			Log::critical("Node vector is not sorted correctly!", LOGINFO);
			return -1;
		}

		index = (startSub + endSub) / 2;
	}

	return index;
}

// static uint32_t fileCount = 1;
static bool hasWarned = false;

// NOTE: This is meant to deal with a number of situations in which to use A star, so if the grid is made to only be a portion of the map, the conversion must happen beforehand
template <size_t width, size_t height, size_t numOfPoints>
inline std::vector<Vec2f> *aStarAlgorithm(Vec2i startPos, Vec2i destPos, CollisionBox box, std::array<Vec2i, numOfPoints> &offsets, std::function<bool(int, int, int, int, CollisionBox)> &collisionDetection, std::function<Vec2f(Vec2i)> &convert, int pathLimit)
{

	std::vector<Vec2f> *path = new std::vector<Vec2f>();

	if(startPos.x != destPos.x || startPos.y != destPos.y)
	{
		// std::stringstream log;
		// log << "NOTE START" <<  startPos << "\n";
		// log << "NOTE DESTINATION" <<  destPos << "\n";

		if(collisionDetection(destPos.x, destPos.y, 0, 0, box))
		{
			// Log::error("Cannot reach destination!", LOGINFO);
			std::vector<Vec2f> *path = new std::vector<Vec2f>();
			path->push_back(convert(destPos));
			return path;
		}
		std::vector<Vec2i> openList;
		openList.reserve(width * height);

		bool closedList[width][height];
		memset(closedList, false, sizeof(closedList));

		std::array<std::array<Node, height>, width> nodeMap;

		{
			float hCost                            = distBetweenVec2i(startPos, destPos);
			nodeMap[startPos.x][startPos.y].parent = {-1, -1};
			nodeMap[startPos.x][startPos.y].fCost  = hCost;
			nodeMap[startPos.x][startPos.y].gCost  = 0.0f;
			nodeMap[startPos.x][startPos.y].hCost  = hCost;

			openList.push_back(startPos);
		}

		auto skipThisPos = [&](Vec2i nextPos, Vec2i offset) -> bool {
			if(nextPos.x >= width || nextPos.x < 0 || nextPos.y >= height || nextPos.y < 0)
			{
				// log << "REJECTED: out of bounds\n";
				return true;
			}

			if(collisionDetection(nextPos.x - offset.x, nextPos.y - offset.y, offset.x, offset.y, box))
			{
				// log << "REJECTED: collision detected\n";
				return true;
			}

			if(closedList[nextPos.x][nextPos.y])
			{
				// log << "REJECTED: Node has been closed\n";
				return true;
			}
			return false;
		};

		while(openList.back() != destPos)
		{
			Vec2i currentPos  = openList.back();
			openList.pop_back();
			// log << "Current: " <<  currentPos << "\n";

			Node *currentNode = &nodeMap[currentPos.x][currentPos.y];
			closedList[currentPos.x][currentPos.y] = true;
			for(Vec2i offset : offsets)
			{
				Vec2i nextPos = {currentPos.x + offset.x, currentPos.y + offset.y};
				// log << "NextPos: " <<  nextPos << "\n";

				if(skipThisPos(nextPos, offset))
					continue;

				float gCost = currentNode->gCost + distBetweenVec2i(currentPos, nextPos);
				float hCost = distBetweenVec2i(nextPos, destPos);
				float fCost = gCost + hCost;
				if(nodeMap[nextPos.x][nextPos.y].fCost != -1)
				{
					// log << "Node already explored\n";
					if(fCost >= nodeMap[nextPos.x][nextPos.y].fCost)
					{
						// log << "REJECTED: has a higher fCost!\n";
						continue;
					}
					nodeMap[nextPos.x][nextPos.y].parent = currentPos;
					nodeMap[nextPos.x][nextPos.y].fCost  = fCost;
					nodeMap[nextPos.x][nextPos.y].gCost  = gCost;
					nodeMap[nextPos.x][nextPos.y].hCost  = hCost;
					openList.erase(std::find(openList.begin(), openList.end(), nextPos));
				}
				else
				{
					// nodeMap[currentPos.x + dirVec.x][currentPos.y + dirVec.y].vec    = nextPos;
					nodeMap[nextPos.x][nextPos.y].parent = currentPos;
					nodeMap[nextPos.x][nextPos.y].fCost  = fCost;
					nodeMap[nextPos.x][nextPos.y].gCost  = gCost;
					nodeMap[nextPos.x][nextPos.y].hCost  = hCost;
				}

				int insertIndex = getIndexOfInsertion(openList, nodeMap, nextPos);

				openList.insert(openList.begin() + insertIndex, nextPos);
				// log << "Inserted node to list at: " << insertIndex << "\n";
			}
			if(openList.size() == 0)
			{
				if(!hasWarned)
				{
					// Log::warning("Cannot find route to destination");
					hasWarned = true;
					// std::stringstream filenameStream;
					// filenameStream << "A Star Log " << fileCount << ".log";
					// fileCount++;

					// std::ofstream file(filenameStream.str(), std::ios_base::app);   // The std::ios_base::app allows it to write at end
					// file << log.str() << "Vector size is 0";
					// file.close();
				}
				std::vector<Vec2f> *path = new std::vector<Vec2f>();
				path->push_back(convert(destPos));
				return path;
			}
			else if(openList.size() > width * height)
			{
				Log::critical("Too many nodes in the open list! Something is wrong!", LOGINFO);
				std::vector<Vec2f> *path = new std::vector<Vec2f>();
				path->push_back(convert(destPos));
				return path;
			}
		}

		Vec2i currentPos = openList.back();
		while(currentPos != startPos)
		{
			Node *currentNode = &nodeMap[currentPos.x][currentPos.y];
			path->push_back(convert(currentPos));
			currentPos = currentNode->parent;
			if(path->size() > pathLimit)
			{
				Log::info("Path too long, cancelling");
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