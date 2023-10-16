#include "StatBar.h"

#include "Application.h"
#include "rendering/Renderer.h"

#include "event/input/Mouse.h"

StatBar::StatBar(std::string name, float x, float y, float width, float height, Layer *layer, const float *stat, const float *statMax)
	: MenuObject(x, y, width, height, layer),
	  m_Name(name),
	  m_State(Button::State::None),
	  m_Stat(stat),
	  m_StatMax(statMax),
	  m_BackgroundColour({1.0f, 0.0f, 0.0f, 1.0f})
{
}

StatBar::StatBar(std::string name, std::function<void(float *, float *, float *, float *)> posFunc, Layer *layer, const float *stat, const float *statMax)
	: MenuObject(posFunc, layer),
	  m_Name(name),
	  m_State(Button::State::None),
	  m_Stat(stat),
	  m_StatMax(statMax),
	  m_BackgroundColour({1.0f, 0.0f, 0.0f, 1.0f})
{
}

StatBar::StatBar(std::string name, float x, float y, float width, float height, Layer *layer, const float *stat, const float *statMax, glm::vec4 backgroundColour)
	: MenuObject(x, y, width, height, layer),
	  m_Name(name),
	  m_State(Button::State::None),
	  m_Stat(stat),
	  m_StatMax(statMax),
	  m_BackgroundColour(backgroundColour)
{
}

StatBar::StatBar(std::string name, std::function<void(float *, float *, float *, float *)> posFunc, Layer *layer, const float *stat, const float *statMax, glm::vec4 backgroundColour)
	: MenuObject(posFunc, layer),
	  m_Name(name),
	  m_State(Button::State::None),
	  m_Stat(stat),
	  m_StatMax(statMax),
	  m_BackgroundColour(backgroundColour)
{
}

StatBar::~StatBar()
{
	// NOTE: m_Stat and m_StatMax should be references to variables stored elsewhere, thus they should not be deleted here!
}

void StatBar::update()
{
	// Updates its state
	Vec2f mousePos = Event::getMousePos();
	if(mousePos.x > x - width / 2 && mousePos.x < x + width / 2 && mousePos.y > y - height / 2 && mousePos.y < y + height / 2)
		m_State = Button::State::Hover;
	else
		m_State = Button::State::None;
}

void StatBar::render()
{
	// Renders the bar
	uint8_t layer = 7;
	Render::rectangle(x, y, 0.0f, width * (*m_Stat) / (*m_StatMax), height, m_BackgroundColour, layer, true, true);

	if(m_State == Button::State::Hover)
	{
		// If the mouse is hovering over it it will render the name
		float scale    = 35.0f * Application::getGUIScale();
		Vec2f mousePos = Event::getMousePos();

		Render::hoverText(m_Name, mousePos.x, mousePos.y, scale, {1.0f, 1.0f, 1.0f, 1.0f}, {0.3f, 0.3f, 0.3f, 0.7f}, layer + 1, true);
	}
}

bool StatBar::eventCallback(const Event::Event &e)
{
	return MenuObject::eventCallback(e);
}

void StatBar::setStat(float *stat, float *statMax)
{
	m_Stat    = stat;
	m_StatMax = statMax;
}
