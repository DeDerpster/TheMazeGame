#pragma once

#include "2dVec.h"

struct CollisionBox
{
	Vec2f lowerBound, upperBound;
};   // Lowerbound is the Bottom Left, upperbound is the top right

bool doesPointIntersectWithBox(Vec2f point, Vec2f pos, CollisionBox box);
bool doesBoxIntersectWithBox(Vec2f pos1, CollisionBox box1, Vec2f pos2, CollisionBox box2);
