#pragma once

enum Direction : uint8_t
{
	north = 0,
	south,
	east,
	west
};

double    directionToRotation(Direction dir);
Direction rotationToDirection(double rotation);
Direction revertDirection(Direction dir);