#include "Text.h"

#include "Application.h"
#include "rendering/Renderer.h"

Text::Text(std::string text)
	: x(0.0f), y(0.0f), m_Text(text), m_Scale(50.0f), m_Colour({1.0f, 1.0f, 1.0f, 1.0f}), isCentered(true), isOverlay(true)
{
}

Text::Text(std::string text, float x, float y, float scale, glm::vec4 colour, bool isCentered, bool isOverlay)
	: x(x), y(y), m_Text(text), m_Scale(scale), m_Colour(colour), isCentered(isCentered), isOverlay(isOverlay)
{
}

Text::~Text()
{
}

void Text::render(float xOffset, float yOffset, uint8_t layer)
{
	// Renders the text with the given offset
	// NOTE: The text automatically scales due to the GUI Scale
	Render::text(m_Text, x + xOffset, y + yOffset, m_Scale * Application::getGUIScale(), m_Colour, layer, isCentered, isOverlay);
}