#pragma once

#include <GLM.h>
#include <array>

#include "RenderVertex.h"
#include "rendering/glInterface/Texture.h"
#include "rendering/sprite/Sprite.h"

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
	std::array<Vertex, 4> convertToVertices();
};

struct ColouredObject : public RenderObject
{
	glm::vec4 colour;

	ColouredObject() {}
	ColouredObject(glm::vec2 position, float width, float height, double rotation, bool centered, glm::vec4 colour)
		: RenderObject(position, width, height, rotation, centered), colour(colour) {}

	// Gets the size of each array of vertices returned by 'convertToColouredVertices'
	virtual uint32_t              getSizeOfVertices() override { return 4 * sizeof(ColouredVertex); }
	std::array<ColouredVertex, 4> convertToColouredVertices();
};

struct TexturedObject : public RenderObject
{
	Sprite::ID spriteID;

	TexturedObject() {}
	TexturedObject(glm::vec2 position, float width, float height, double rotation, bool centered, Sprite::ID spriteID)
		: RenderObject(position, width, height, rotation, centered), spriteID(spriteID) {}

	// Gets the size of each array of vertices returned by 'convertToTexturedVertices'
	virtual uint32_t              getSizeOfVertices() override { return 4 * sizeof(TexturedVertex); }
	std::array<TexturedVertex, 4> convertToTexturedVertices(uint8_t texSlot);
};

struct TextObject : public ColouredObject
{
	std::string text;
	float       scale;

	TextObject(std::string text, float scale, glm::vec2 position, float width, float height, double rotation, glm::vec4 colour, bool centered)
		: ColouredObject(position, width, height, rotation, centered, colour), text(text), scale(scale) {}

	// Gets the size of each array of vertices returned by 'convertToCharacterVertices'
	virtual uint32_t          getSizeOfVertices() override { return 4 * sizeof(TextVertex); }
	// This function is slightly different from the rest, however this is because it stores a string and so is rendered
	// by going through each character, so this takes in the character and the offset when creating the vertices
	// This allows to correctly render rotated strings
	std::array<TextVertex, 4> convertCharacterToVertices(Character *ch, float xOffset, uint8_t texSlot);
};