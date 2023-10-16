#include "CollisionBox.h"

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