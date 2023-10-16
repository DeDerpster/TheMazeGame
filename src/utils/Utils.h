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
	Vec2f vec;
	Node *parent;
	float fCost, gCost, hCost;

	Node(Vec2f vec, Node *parent, double gCost, double hCost)
		: vec(vec), parent(parent), fCost(gCost + hCost), gCost(gCost), hCost(hCost) {}
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

static int getIndexOfInsertion(std::vector<Node *> &nodes, Node &node)   // Gets index of place to put node for a vector sorted Most cost -> Small cost
{
	if(nodes.size() == 0)
		return 0;
	if(nodes.size() == 1)
	{
		if(*nodes[0] > node)
			return 1;
		else
			return 0;
	}
	int startSub = 0;
	int endSub   = nodes.size();
	int index    = nodes.size() / 2;

	while(startSub != endSub && startSub < endSub)
	{
		if(index + 1 == nodes.size())
			index--;
		if(*nodes[index] == node || (*nodes[index] > node && *nodes[index + 1] < node))
			return index + 1;
		else if(*nodes[index + 1] == node)
			return index + 2;
		else if(*nodes[index] > node && *nodes[index + 1] > node)
			startSub = index + 2;
		else if(*nodes[index] < node && *nodes[index + 1] < node)
			endSub = index;
		else
		{
			Log::critical("Node vector is not sorted correctly!", LOGINFO);
			return -1;
		}

		index = (startSub + endSub) / 2;
	}

	// Log::debug("Got to the end of array");
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