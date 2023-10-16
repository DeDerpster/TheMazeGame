#include "Text.h"

#include "Renderer.h"

Text::Text(std::string text)
	: x(0.0f), y(0.0f), m_Text(text), m_Scale(50.0f), m_Colour({1.0f, 1.0f, 1.0f, 1.0f}), centerX(true), centerY(true), isOverlay(true)
{
}

Text::Text(std::string text, float x, float y, float scale, glm::vec4 colour, bool centerX, bool centerY, bool isOverlay)
	: x(x), y(y), m_Text(text), m_Scale(scale), m_Colour(colour), centerX(centerX), centerY(centerY), isOverlay(isOverlay)
{
}

Text::~Text()
{
}

void Text::render(float xOffset, float yOffset)
{
	Render::text(m_Text, x + xOffset, y + yOffset, m_Scale, m_Colour, centerX, centerY, isOverlay);
}