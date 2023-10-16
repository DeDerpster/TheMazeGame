#include "Direction.h"

#include "Log.h"

#define _USE_MATH_DEFINES
#include <math.h>

double directionToRotation(Direction dir)
{
	switch(dir)
	{
	case Direction::north:
		return 0.0f;
	case Direction::south:
		return M_PI;
	case Direction::east:
		return M_PI_2;
	case Direction::west:
		return 3.0f * M_PI_2;
	default:
		Log::warning("Unknown direction when converting into a rotation");
		return 0.0f;
	}
}

Direction rotationToDirection(double rotation)
{
	if(rotation == 0.0f)
		return north;
	else if(rotation == M_PI)
		return south;
	else if(rotation == M_PI_2)
		return east;
	else if(rotation == 3.0f * M_PI_2)
		return west;
	else
		Log::warning("Unknown rotation when converting into a direction!");
	return north;
}

Direction revertDirection(Direction dir)
{
	switch(dir)
	{
	case north:
		return south;
	case south:
		return north;
	case east:
		return west;
	case west:
		return east;
	default:
		Log::warning("Unknown direction when reverting the direction");
		return north;
	}
}