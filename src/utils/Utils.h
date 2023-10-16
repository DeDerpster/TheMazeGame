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

enum class RoomType
{
	Empty = 0,
	Chest,
	Trap,
	Enemy,
	NPC,
	Exit
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

inline Vec2f operator+(const Vec2f &lhs, const Vec2f &rhs)
{
	return {lhs.x + rhs.x, lhs.y + rhs.y};
}
inline Vec2f operator+(const Vec2f &lhs, const float &rhs)
{
	return {lhs.x + rhs, lhs.y + rhs};
}
inline Vec2f operator-(const Vec2f &lhs, const Vec2f &rhs)
{
	return {lhs.x - rhs.x, lhs.y - rhs.y};
}
inline Vec2f operator-(const Vec2f &lhs, const float &rhs)
{
	return {lhs.x - rhs, lhs.y - rhs};
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

// TODO: Change this into a template
float distBetweenVec2i(const Vec2i &start, const Vec2i &end);
float distBetweenVec2f(const Vec2f &start, const Vec2f &end);

bool doesPointIntersectWithBox(Vec2f point, Vec2f pos, CollisionBox box);
bool doesBoxIntersectWithBox(Vec2f pos1, CollisionBox box1, Vec2f pos2, CollisionBox box2);