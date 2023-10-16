#include "RenderObject.h"

std::array<Vertex, 4> RenderObject::convertToVertices()
{
	// Creates a 2d rotation matrix, so that the object can be rotated
	glm::mat2 rotationMatrix({glm::cos(rotation), -glm::sin(rotation)}, {glm::sin(rotation), glm::cos(rotation)});

	// Gets the point at which to rotate around
	float leftPoint, rightPoint, topPoint, bottomPoint;
	if(centered)
	{
		// This centers the object
		float xHalfSize = width / 2;
		float yHalfSize = height / 2;
		leftPoint       = -xHalfSize;
		rightPoint      = xHalfSize;
		topPoint        = yHalfSize;
		bottomPoint     = -yHalfSize;
	}
	else
	{
		leftPoint   = 0;
		rightPoint  = width;
		topPoint    = height;
		bottomPoint = 0;
	}

	// Creates 4 vertices that create the square
	Vertex v0(rotationMatrix * glm::vec2(leftPoint, bottomPoint) + position);
	Vertex v1(rotationMatrix * glm::vec2(rightPoint, bottomPoint) + position);
	Vertex v2(rotationMatrix * glm::vec2(rightPoint, topPoint) + position);
	Vertex v3(rotationMatrix * glm::vec2(leftPoint, topPoint) + position);

	return {v0, v1, v2, v3};
}

std::array<ColouredVertex, 4> ColouredObject::convertToColouredVertices()
{
	// Creates a 2d rotation matrix, so that the object can be rotated
	glm::mat2 rotationMatrix({glm::cos(rotation), -glm::sin(rotation)}, {glm::sin(rotation), glm::cos(rotation)});

	// Gets the point at which to rotate around
	float leftPoint, rightPoint, topPoint, bottomPoint;
	if(centered)
	{
		// This centers the object
		float xHalfSize = width / 2;
		float yHalfSize = height / 2;

		leftPoint   = -xHalfSize;
		rightPoint  = xHalfSize;
		topPoint    = yHalfSize;
		bottomPoint = -yHalfSize;
	}
	else
	{
		leftPoint   = 0;
		rightPoint  = width;
		topPoint    = height;
		bottomPoint = 0;
	}

	// Creates 4 vertices that create the rectangle
	ColouredVertex v0(
		rotationMatrix * glm::vec2(leftPoint, bottomPoint) + position,
		colour);
	ColouredVertex v1(
		rotationMatrix * glm::vec2(rightPoint, bottomPoint) + position,
		colour);
	ColouredVertex v2(
		rotationMatrix * glm::vec2(rightPoint, topPoint) + position,
		colour);
	ColouredVertex v3(
		rotationMatrix * glm::vec2(leftPoint, topPoint) + position,
		colour);

	return {v0, v1, v2, v3};
}

std::array<TexturedVertex, 4> TexturedObject::convertToTexturedVertices(uint16_t texSlot)
{
	// Creates a 2d rotation matrix, so that the object can be rotated
	glm::mat2 rotationMatrix({glm::cos(rotation), -glm::sin(rotation)}, {glm::sin(rotation), glm::cos(rotation)});

	// Gets the point at which to rotate around
	float leftPoint, rightPoint, topPoint, bottomPoint;
	if(centered)
	{
		// This centers the object
		float xHalfSize = width / 2;
		float yHalfSize = height / 2;
		leftPoint       = -xHalfSize;
		rightPoint      = xHalfSize;
		topPoint        = yHalfSize;
		bottomPoint     = -yHalfSize;
	}
	else
	{
		leftPoint   = 0;
		rightPoint  = width;
		topPoint    = height;
		bottomPoint = 0;
	}

	// Creates 4 vertices that create the sprite
	TexturedVertex v0(
		rotationMatrix * glm::vec2(leftPoint, bottomPoint) + position,
		{0.0f, 0.0f},
		texSlot);

	TexturedVertex v1(
		rotationMatrix * glm::vec2(rightPoint, bottomPoint) + position,
		{1.0f, 0.0f},
		texSlot);

	TexturedVertex v2(
		rotationMatrix * glm::vec2(rightPoint, topPoint) + position,
		{1.0f, 1.0f},
		texSlot);

	TexturedVertex v3(
		rotationMatrix * glm::vec2(leftPoint, topPoint) + position,
		{0.0f, 1.0f},
		texSlot);

	return {v0, v1, v2, v3};
}

std::array<TextVertex, 4> TextObject::convertCharacterToVertices(Character *ch, float xOffset, uint16_t texSlot)
{
	float newScale = scale / 100;

	float xPos = position.x + ch->bearing.x * newScale + xOffset;
	float yPos = position.y - (ch->size.y - ch->bearing.y) * newScale;

	float w = ch->size.x * newScale;
	float h = ch->size.y * newScale;

	// Creates a 2d rotation matrix, so that the object can be rotated
	glm::mat2 rotationMatrix({glm::cos(rotation), -glm::sin(rotation)}, {glm::sin(rotation), glm::cos(rotation)});

	// Gets the point at which to rotate around
	float leftPoint, rightPoint, topPoint, bottomPoint;
	if(centered)
	{
		// This centers the object
		float xHalfSize = width / 2;
		float yHalfSize = height / 2;
		leftPoint       = -xHalfSize;
		rightPoint      = -xHalfSize + w;
		topPoint        = -yHalfSize + h;
		bottomPoint     = -yHalfSize;
	}
	else
	{
		leftPoint   = 0;
		rightPoint  = w;
		topPoint    = h;
		bottomPoint = 0;
	}

	// Creates 4 vertices that create the character
	TextVertex v0(
		rotationMatrix * glm::vec2(leftPoint, topPoint) + glm::vec2(xPos, yPos),
		{0.0f, 0.0f},
		texSlot,
		colour);
	TextVertex v1(
		rotationMatrix * glm::vec2(rightPoint, topPoint) + glm::vec2(xPos, yPos),
		{1.0f, 0.0f},
		texSlot,
		colour);
	TextVertex v2(
		rotationMatrix * glm::vec2(rightPoint, bottomPoint) + glm::vec2(xPos, yPos),
		{1.0f, 1.0f},
		texSlot,
		colour);
	TextVertex v3(
		rotationMatrix * glm::vec2(leftPoint, bottomPoint) + glm::vec2(xPos, yPos),
		{0.0f, 1.0f},
		texSlot,
		colour);

	return {v0, v1, v2, v3};
}