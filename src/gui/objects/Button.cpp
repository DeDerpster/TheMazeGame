#include "Button.h"

#include "Renderer.h"

Button::Button(Text text, float x, float y, float width, float height, std::function<void()> buttonPressFunc)
	: MenuObject(x, y, width, height),
	  m_Text(text),
	  m_BackgroundColour({0.3f, 0.3f, 0.3f, 1.0f}),
	  m_HoverColour({0.2f, 0.2f, 0.2f, 1.0f}),
	  m_PressColour({0.0f, 0.0f, 0.0f, 1.0f}),
	  m_BorderColour({0.0f, 0.0f, 0.0f, 1.0f}),
	  pressCalldown(0),
	  m_State(State::None),
	  buttonPressFunc(buttonPressFunc)
{
}

Button::Button(Text text, std::function<void(float *, float *, float *, float *)> posFunc, std::function<void()> buttonPressFunc)
	: MenuObject(posFunc),
	  m_Text(text),
	  m_BackgroundColour({0.3f, 0.3f, 0.3f, 1.0f}),
	  m_HoverColour({0.2f, 0.2f, 0.2f, 1.0f}),
	  m_PressColour({0.0f, 0.0f, 0.0f, 1.0f}),
	  m_BorderColour({0.0f, 0.0f, 0.0f, 1.0f}),
	  pressCalldown(0),
	  m_State(State::None),
	  buttonPressFunc(buttonPressFunc)
{
}

Button::Button(Text text, float x, float y, float width, float height, glm::vec4 backgroundColour, glm::vec4 hoverColour, glm::vec4 pressColour, std::function<void()> buttonPressFunc)
	: MenuObject(x, y, width, height),
	  m_Text(text),
	  m_BackgroundColour(backgroundColour),
	  m_HoverColour(hoverColour),
	  m_PressColour(pressColour),
	  m_BorderColour({0.0f, 0.0f, 0.0f, 1.0f}),
	  pressCalldown(0),
	  m_State(State::None),
	  buttonPressFunc(buttonPressFunc)
{
}

Button::Button(Text text, std::function<void(float *, float *, float *, float *)> posFunc, glm::vec4 backgroundColour, glm::vec4 hoverColour, glm::vec4 pressColour, std::function<void()> buttonPressFunc)
	: MenuObject(posFunc),
	  m_Text(text),
	  m_BackgroundColour(backgroundColour),
	  m_HoverColour(hoverColour),
	  m_PressColour(pressColour),
	  m_BorderColour({0.0f, 0.0f, 0.0f, 1.0f}),
	  pressCalldown(0),
	  m_State(State::None),
	  buttonPressFunc(buttonPressFunc)
{
}

Button::~Button()
{
}

void Button::update()
{
	if(pressCalldown > 0)
		pressCalldown--;
	else
	{
		Vec2f mousePos = Event::getMousePos();
		if(mousePos.x > x - width / 2 && mousePos.x < x + width / 2 && mousePos.y > y - height / 2 && mousePos.y < y + height / 2)
			m_State = State::Hover;
		else
			m_State = State::None;
	}
}

void Button::render()
{
	if(m_State == State::None)
		Render::rectangle(x, y, width, height, m_BackgroundColour, 2.0f, m_BorderColour, true, true);
	else if(m_State == State::Hover)
		Render::rectangle(x, y, width, height, m_HoverColour, 2.0f, m_BorderColour, true, true);
	else
		Render::rectangle(x, y, width, height, m_PressColour, 2.0f, m_BorderColour, true, true);

	m_Text.render(x, y);
}

bool Button::eventCallback(const Event::Event &e)
{
	if(e.getType() == Event::EventType::mouseClicked && pressCalldown == 0)
	{
		const Event::MouseClickedEvent &ne = static_cast<const Event::MouseClickedEvent &>(e);

		Vec2f mousePos = ne.pos;
		if(ne.button == Event::leftButton && mousePos.x > x - width / 2 && mousePos.x < x + width / 2 && mousePos.y > y - height / 2 && mousePos.y < y + height / 2)
		{
			m_State = State::Press;
			buttonPressFunc();
			pressCalldown = 10;
		}
		return true;
	}

	return MenuObject::eventCallback(e);
}
