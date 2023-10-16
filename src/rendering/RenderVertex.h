#pragma once

#include <GLM.h>

struct Vertex
{
	glm::vec2 position;

	Vertex() {}
	Vertex(glm::vec2 position)
		: position(position) {}
};

struct TexturedVertex : public Vertex
{
	glm::vec2 texCoords;
	float     texID;

	TexturedVertex() {}
	TexturedVertex(glm::vec2 position, glm::vec2 texCoords, float texID)
		: Vertex(position), texCoords(texCoords), texID(texID) {}
};

struct ColouredVertex : public Vertex
{
	glm::vec4 colour;

	ColouredVertex() {}
	ColouredVertex(glm::vec2 position, glm::vec4 colour)
		: Vertex(position), colour(colour) {}
};

struct TextVertex : public TexturedVertex
{
	glm::vec4 colour;

	TextVertex() {}
	TextVertex(glm::vec2 position, glm::vec2 texCoords, float texID, glm::vec4 colour)
		: TexturedVertex(position, texCoords, texID), colour(colour) {}
};