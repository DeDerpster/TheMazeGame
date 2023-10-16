#pragma once

#include <GLM.h>
#include <string>

struct Text
{
	// Stores the relative position
	float x, y;

	std::string m_Text;     // Stores the text
	float       m_Scale;    // Stores the scale of the text
	glm::vec4   m_Colour;   // Stores the colour of the text

	// Stores some settings for when rendering
	bool isCentered, isOverlay;

	Text(std::string text);
	Text(std::string text, float x, float y, float scale, glm::vec4 colour, bool isCentered = true, bool isOverlay = true);
	~Text();

	void render(float xOffset, float yOffset, uint8_t layer);
};