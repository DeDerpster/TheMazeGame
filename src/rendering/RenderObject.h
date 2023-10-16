#pragma once

#include <GLM.h>
#include <array>

#include "RenderVertex.h"
#include "Texture.h"

// Text rendering objects
struct Character
{
	Texture *  texture;
	glm::ivec2 size;      // Size of glyph
	glm::ivec2 bearing;   // Offset from baseline to left/top of glyph
	uint16_t   advance;   // Offset to advance to next glyph

	~Character()
	{
		if(texture)
			delete texture;
	}
};

// These are the objects that are stored in the buffer, and store all the information needed so they can be converted and transferred to the vertex buffer
// This is most definitely less efficient then just converting them and storing them in the buffer
// However, this allows me to easily order and manipulate them while stored in the buffer,
// so I believe the trade off is worth it

struct RenderObject
{
	glm::vec2 position;
	float     width, height;
	double    rotation;
	bool      centered;

	RenderObject() {}
	RenderObject(glm::vec2 position, float width, float height, double rotation, bool centered)
		: position(position), width(width), height(height), rotation(rotation), centered(centered) {}

	// Gets the size of each array of vertices returned by 'convertToVertices'
	virtual uint32_t      getSizeOfVertices() { return 4 * sizeof(Vertex); }
	std::array<Vertex, 4> convertToVertices()
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
};

struct RenderColouredObject : public RenderObject
{
	glm::vec4 colour;

	RenderColouredObject() {}
	RenderColouredObject(glm::vec2 position, float width, float height, double rotation, bool centered, glm::vec4 colour)
		: RenderObject(position, width, height, rotation, centered), colour(colour) {}

	// Gets the size of each array of vertices returned by 'convertToColouredVertices'
	virtual uint32_t              getSizeOfVertices() override { return 4 * sizeof(ColouredVertex); }
	std::array<ColouredVertex, 4> convertToColouredVertices()
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
};

struct TexturedObject : public RenderObject
{
	uint16_t spriteID;

	TexturedObject() {}
	TexturedObject(glm::vec2 position, float width, float height, double rotation, bool centered, uint16_t spriteID)
		: RenderObject(position, width, height, rotation, centered), spriteID(spriteID) {}

	// Gets the size of each array of vertices returned by 'convertToTexturedVertices'
	virtual uint32_t              getSizeOfVertices() override { return 4 * sizeof(TexturedVertex); }
	std::array<TexturedVertex, 4> convertToTexturedVertices(uint16_t texSlot)
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
};

struct TextObject : public RenderColouredObject
{
	std::string text;
	float       scale;

	TextObject(std::string text, float scale, glm::vec2 position, float width, float height, double rotation, glm::vec4 colour, bool centered)
		: RenderColouredObject(position, width, height, rotation, centered, colour), text(text), scale(scale) {}

	// Gets the size of each array of vertices returned by 'convertToCharacterVertices'
	virtual uint32_t          getSizeOfVertices() override { return 4 * sizeof(TextVertex); }
	// This function is slightly different from the rest, however this is because it stores a string and so is rendered
	// by going through each character, so this takes in the character and the offset when creating the vertices
	// This allows to correctly render rotated strings
	std::array<TextVertex, 4> convertCharacterToVertices(Character *ch, float xOffset, uint16_t texSlot)
	{   // FIXME: rendering isn't working for 'g' when centered on y axis
		float newScale = scale / 100;

		float xPos = position.x + ch->bearing.x * newScale;
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
			leftPoint       = -xHalfSize + xOffset;
			rightPoint      = -xHalfSize + xOffset + w;
			topPoint        = -yHalfSize + h;
			bottomPoint     = -yHalfSize;
		}
		else
		{
			leftPoint   = xOffset;
			rightPoint  = w + xOffset;
			topPoint    = h;
			bottomPoint = 0;
		}

		// Creates 4 vertices that create the character
		TextVertex v0(
			rotationMatrix * glm::vec2(leftPoint, topPoint) + position,
			{0.0f, 0.0f},
			texSlot,
			colour);
		TextVertex v1(
			rotationMatrix * glm::vec2(rightPoint, topPoint) + position,
			{1.0f, 0.0f},
			texSlot,
			colour);
		TextVertex v2(
			rotationMatrix * glm::vec2(rightPoint, bottomPoint) + position,
			{1.0f, 1.0f},
			texSlot,
			colour);
		TextVertex v3(
			rotationMatrix * glm::vec2(leftPoint, bottomPoint) + position,
			{0.0f, 1.0f},
			texSlot,
			colour);

		return {v0, v1, v2, v3};
	}
};