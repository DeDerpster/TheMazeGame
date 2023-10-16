#include "Utils.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include "level/Level.h"

uint32_t factorial(int num)
{
	uint32_t output = 1;
	for(uint16_t i = 1; i <= num; i++)
		output *= i;
	return output;
}

float distBetweenVec2f(const Vec2f &start, const Vec2f &end)
{
	float xDist = end.x - start.x;
	float yDist = end.y - start.y;

	return std::sqrt(xDist * xDist + yDist * yDist);   // Uses Pythagorus
}

float distBetweenVec2i(const Vec2i &start, const Vec2i &end)
{
	float xDist = end.x - start.x;
	float yDist = end.y - start.y;

	return std::sqrt(xDist * xDist + yDist * yDist);   // Uses Pythagorus
}

float directionToRotation(Direction dir)
{
	if(dir == Direction::north)
		return 0.0f;
	else if(dir == Direction::south)
		return M_PI;
	else if(dir == Direction::east)
		return M_PI_2;
	else
		return 3.0f * M_PI_2;
}

bool doesPointIntersectWithBox(Vec2f point, Vec2f pos, CollisionBox box)
{
	float lowerX = pos.x + box.lowerBound.x;
	float lowerY = pos.y + box.lowerBound.y;
	float upperX = pos.x + box.upperBound.x;
	float upperY = pos.y + box.upperBound.y;

	return point.x > lowerX && point.y > lowerY && point.x < upperX && point.y < upperY;
}

bool doesBoxIntersectWithBox(Vec2f pos1, CollisionBox box1, Vec2f pos2, CollisionBox box2)
{
	return !(pos1.x + box1.upperBound.x < pos2.x + box2.lowerBound.x || pos1.x + box1.lowerBound.x > pos2.x + box2.upperBound.x || pos1.y + box1.upperBound.y < pos2.y + box2.lowerBound.y || pos1.y + box1.lowerBound.y > pos2.y + box2.upperBound.y);
}