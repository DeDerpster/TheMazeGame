#pragma once

#include <math.h>

// Simple integer vector
struct Vec2i
{
	int x, y;

	// Returns the sum of the numbers squared
	float squareSum() { return x * x + y * y; }
};

// Boolean expressions
inline bool operator!=(const Vec2i &lhs, const Vec2i &rhs)
{
	return lhs.x != rhs.x || lhs.y != rhs.y;
}

inline bool operator==(const Vec2i &lhs, const Vec2i &rhs)
{
	return lhs.x == rhs.x && lhs.y == rhs.y;
}

// Basic maths operations
inline Vec2i operator-(const Vec2i &lhs, const Vec2i &rhs)
{
	return {lhs.x - rhs.x, lhs.y - rhs.y};
}

inline Vec2i operator+(const Vec2i &lhs, const Vec2i &rhs)
{
	return {lhs.x + rhs.x, lhs.y + rhs.y};
}

// Printing (for logging)
inline std::ostream &operator<<(std::ostream &out, const Vec2i &data)
{
	out << "(" << data.x << ", " << data.y << ")";
	return out;
}

// Simple float vector
struct Vec2f
{
	float x, y;

	// Returns the sum of the numbers squared
	float squareSum() { return x * x + y * y; }
};

// Boolean operations
inline bool operator!=(const Vec2f &lhs, const Vec2f &rhs)
{
	return lhs.x != rhs.x || lhs.y != rhs.y;
}

inline bool operator==(const Vec2f &lhs, const Vec2f &rhs)
{
	return lhs.x == rhs.x && lhs.y == rhs.y;
}

// Basic maths operations
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

// Printing (for logging)
inline std::ostream &operator<<(std::ostream &out, const Vec2f &data)
{
	out << "(" << data.x << ", " << data.y << ")";
	return out;
}

// Uses pythagoras theorem to work out the distance between two points
template <typename T>
float distBetweenVec(const T &start, const T &end)
{
	T difference = start - end;
	return std::sqrt(difference.squareSum());
}