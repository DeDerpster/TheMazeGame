#pragma once

#include <GLM.h>

// This is all the vertices, set in the correct layout to be directly copied to the vertex buffer without any alterations
// Each type of vertex is for a different shader
struct Vertex
{
	glm::vec2 position;

	Vertex() {}
	Vertex(glm::vec2 position)
		: position(position) {}
};

struct TexturedVertex : public Vertex   // For the sprite shader
{
	glm::vec2 texCoords;
	float     texID;

	TexturedVertex() {}
	TexturedVertex(glm::vec2 position, glm::vec2 texCoords, float texID)
		: Vertex(position), texCoords(texCoords), texID(texID) {}
};

struct ColouredVertex : public Vertex   // For the simple shader
{
	glm::vec4 colour;

	ColouredVertex() {}
	ColouredVertex(glm::vec2 position, glm::vec4 colour)
		: Vertex(position), colour(colour) {}
};

struct TextVertex : public TexturedVertex   // For the text shader
{
	glm::vec4 colour;

	TextVertex() {}
	TextVertex(glm::vec2 position, glm::vec2 texCoords, float texID, glm::vec4 colour)
		: TexturedVertex(position, texCoords, texID), colour(colour) {}
};