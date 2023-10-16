#pragma once

struct Vec2i
{
	int x, y;
};

struct Vec2f
{
	float x, y;
};

struct Node
{
	Vec2i  vec;
	Node * parent;
	double fCost, gCost, hCost;

	Node(Vec2i vec, Node *parent, double gCost, double hCost)
		: vec(vec), parent(parent), fCost(gCost + hCost), gCost(gCost), hCost(hCost) {}
};

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
}