#pragma once

#include <GLM.h>

#include "Log.h"

enum Direction
{
	north = 0,
	south,
	east,
	west
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

inline std::ostream &operator<<(std::ostream &out, const Vec2i &data)
{
	out << "(" << data.x << ", " << data.y << ")";
	return out;
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

inline std::ostream &operator<<(std::ostream &out, const Vec2f &data)
{
	out << "(" << data.x << ", " << data.y << ")";
	return out;
}

struct CollisionBox
{
	Vec2f lowerBound, upperBound;
};   // Lowerbound is the Bottom Left, upperbound is the top right



uint32_t factorial(int num);
float directionToRotation(Direction dir);
float distBetweenVec2i(const Vec2i &start, const Vec2i &end);
float distBetweenVec2f(const Vec2f &start, const Vec2f &end);

