#include "Utils.h"

#define _USE_MATH_DEFINES
#include <math.h>

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