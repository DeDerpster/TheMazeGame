#pragma once

#include <math.h>
#include <vector>

#include "Log.h"

enum Direction
{
	NORTH = 0,
	SOUTH,
	EAST,
	WEST
};

struct Vec2i
{
	int x, y;
};

inline bool operator!=(const Vec2i &lhs, const Vec2i &rhs)
{
	return lhs.x != rhs.x || lhs.y != rhs.y;
}
inline bool operator==(const Vec2i &lhs, const Vec2i &rhs)
{
	return lhs.x == rhs.x && lhs.y == rhs.y;
}
static float distBetweenVec2i(const Vec2i &start, const Vec2i &end)
{
	float xDist = end.x - start.x;
	float yDist = end.y - start.y;

	return std::sqrt(xDist * xDist + yDist * yDist);   // Uses Pythagorus
}

struct Vec2f
{
	float x, y;
};

inline bool operator!=(const Vec2f &lhs, const Vec2f &rhs)
{
	return lhs.x != rhs.x || lhs.y != rhs.y;
}
inline bool operator==(const Vec2f &lhs, const Vec2f &rhs)
{
	return lhs.x == rhs.x && lhs.y == rhs.y;
}

static float distBetweenVec2f(const Vec2f &start, const Vec2f &end)
{
	float xDist = end.x - start.x;
	float yDist = end.y - start.y;

	return std::sqrt(xDist * xDist + yDist * yDist);   // Uses Pythagorus
}

struct CollisionBox
{
	Vec2f lowerBound, upperBound;
};

struct Node
{
	Vec2i vec;
	// Node *parent;
	Vec2i parent;
	float fCost, gCost, hCost;

	Node()
		: vec({-1, -1}), parent({-1, -1}), fCost(0), gCost(0), hCost(0) {}
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

static int nodeSearch(std::vector<Node *> &nodes, Node &node)
{
	for(int i = 0; i < nodes.size(); i++)
	{
		if(nodes[i]->vec.x == node.vec.x && nodes[i]->vec.y == node.vec.y)
			return i;
	}
	return -1;
}

// static int getIndexOfInsertion(std::vector<Node *> &nodes, Node &node)   // Gets index of place to put node for a vector sorted Most cost -> Small cost
template <size_t ySize, size_t xSize>
int getIndexOfInsertion(std::vector<Vec2i> positions, std::array<std::array<Node, ySize>, xSize> &nodeMap, Vec2i nextPos)
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

	Node *node = &nodeMap[nextPos.x][nextPos.y];

	while(startSub != endSub && startSub < endSub)
	{
		if(index + 1 == positions.size())
			index--;

		Node *thisNode = &nodeMap[positions[index].x][positions[index].y];
		Node *nextNode = &nodeMap[positions[index + 1].x][positions[index + 1].y];

		if(*thisNode == *node || (*thisNode > *node && *nextNode < *node))
			return index + 1;
		else if(*nextNode == *node)
			return index + 2;
		else if(*thisNode > *node && *nextNode > *node)
			startSub = index + 2;
		else if(*thisNode < *node && *nextNode < *node)
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

struct Vertex
{
	glm::vec2 position;
	Vec2f     texCoords;
	float     texID;

	Vertex() {}
	Vertex(glm::vec2 position, Vec2f texCoords, float texID)
		: position(position), texCoords(texCoords), texID(texID) {}
};

namespace Sprite
{
	struct Object
	{
		float  x;
		float  y;
		double rotation;
		float  size;
	};
}   // namespace Sprite