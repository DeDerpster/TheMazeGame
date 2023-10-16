#include "Button.h"

#include "rendering/Renderer.h"

#include "event/input/Mouse.h"

Button::Button(Text text, float x, float y, float width, float height, Layer *layer, std::function<void()> buttonPressFunc)
	: MenuObject(x, y, width, height, layer),
	  m_Text(text),
	  m_BackgroundColour({0.3f, 0.3f, 0.3f, 1.0f}),
	  m_HoverColour({0.2f, 0.2f, 0.2f, 1.0f}),
	  m_PressColour({0.0f, 0.0f, 0.0f, 1.0f}),
	  m_BorderColour({0.0f, 0.0f, 0.0f, 1.0f}),
	  m_State(State::None),
	  buttonPressFunc(buttonPressFunc)
{
}

Button::Button(Text text, std::function<void(float *, float *, float *, float *)> posFunc, Layer *layer, std::function<void()> buttonPressFunc)
	: MenuObject(posFunc, layer),
	  m_Text(text),
	  m_BackgroundColour({0.3f, 0.3f, 0.3f, 1.0f}),
	  m_HoverColour({0.2f, 0.2f, 0.2f, 1.0f}),
	  m_PressColour({0.0f, 0.0f, 0.0f, 1.0f}),
	  m_BorderColour({0.0f, 0.0f, 0.0f, 1.0f}),
	  m_State(State::None),
	  buttonPressFunc(buttonPressFunc)
{
}

Button::Button(Text text, float x, float y, float width, float height, Layer *layer, glm::vec4 backgroundColour, glm::vec4 hoverColour, glm::vec4 pressColour, std::function<void()> buttonPressFunc)
	: MenuObject(x, y, width, height, layer),
	  m_Text(text),
	  m_BackgroundColour(backgroundColour),
	  m_HoverColour(hoverColour),
	  m_PressColour(pressColour),
	  m_BorderColour({0.0f, 0.0f, 0.0f, 1.0f}),
	  m_State(State::None),
	  buttonPressFunc(buttonPressFunc)
{
}

Button::Button(Text text, std::function<void(float *, float *, float *, float *)> posFunc, Layer *layer, glm::vec4 backgroundColour, glm::vec4 hoverColour, glm::vec4 pressColour, std::function<void()> buttonPressFunc)
	: MenuObject(posFunc, layer),
	  m_Text(text),
	  m_BackgroundColour(backgroundColour),
	  m_HoverColour(hoverColour),
	  m_PressColour(pressColour),
	  m_BorderColour({0.0f, 0.0f, 0.0f, 1.0f}),
	  m_State(State::None),
	  buttonPressFunc(buttonPressFunc)
{
}

Button::~Button()
{
}

void Button::update()
{
	// Checks to see if the mouse is hovering over it
	Vec2f mousePos = Event::getMousePos();
	if(mousePos.x > x - width / 2 && mousePos.x < x + width / 2 && mousePos.y > y - height / 2 && mousePos.y < y + height / 2)
		m_State = State::Hover;
	else
		m_State = State::None;
}

void Button::render()
{
	// Renders a rectangle with the specific colour of what state it is in
	uint8_t layer = 7;

	switch(m_State)
	{
	case State::Press:
		Render::rectangle(x, y, width, height, m_PressColour, 2.0f, m_BorderColour, layer, true, true);
		break;
	case State::Hover:
		Render::rectangle(x, y, width, height, m_HoverColour, 2.0f, m_BorderColour, layer, true, true);
		break;

	default:
		Render::rectangle(x, y, width, height, m_BackgroundColour, 2.0f, m_BorderColour, layer, true, true);
		break;
	}

	m_Text.render(x, y, layer);
}

bool Button::eventCallback(const Event::Event &e)
{
	switch(e.getType())
	{
	case Event::EventType::MouseClicked:
	{
		// Checks if mouse has been clicked
		const Event::MouseClickedEvent &ne = static_cast<const Event::MouseClickedEvent &>(e);

		if(m_State == State::Hover && ne.action == Event::Action::Press)
		{
			// If it is over the button it will change its state and call the button press function
			m_State = State::Press;
			buttonPressFunc();

			return true;
		}

		return false;
	}

	default:
		return MenuObject::eventCallback(e);
	}
}
