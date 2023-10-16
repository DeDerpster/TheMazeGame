#include "MenuBackgroundObject.h"

#include "rendering/Renderer.h"

#include "event/input/Keyboard.h"
#include "event/input/Mouse.h"

MenuBackground::MenuBackground(float x, float y, float width, float height, Layer *layer, glm::vec4 colour, std::function<void()> exitFunc)
	: MenuObject(x, y, width, height, layer), m_Colour(colour), m_ExitFunc(exitFunc)
{
}

MenuBackground::MenuBackground(std::function<void(float *, float *, float *, float *)> posFunc, Layer *layer, glm::vec4 colour, std::function<void()> exitFunc)
	: MenuObject(posFunc, layer), m_Colour(colour), m_ExitFunc(exitFunc)
{
}

MenuBackground::~MenuBackground()
{
}

void MenuBackground::render()
{
	// Renders the background on layer 6
	uint8_t layer = 6;
	Render::rectangle(x, y, 0.0f, width, height, m_Colour, layer, true, true);
}

void MenuBackground::update()
{
}

bool MenuBackground::eventCallback(const Event::Event &e)
{
	switch(e.getType())
	{
	case Event::EventType::KeyInput:
	{
		const Event::KeyboardEvent &ne = static_cast<const Event::KeyboardEvent &>(e);

		// If the escape key has been pressed it will call the exit function
		if(ne.key == Event::KeyboardKey::Escape && ne.action == Event::Action::Press)
		{
			m_ExitFunc();
			return true;
		}

		return false;
	}

	case Event::EventType::MouseClicked:
	{
		const Event::MouseClickedEvent &ne = static_cast<const Event::MouseClickedEvent &>(e);

		// If the mouse has been clicked outside the background it will call the exit function
		if(ne.button == Event::MouseButton::LeftButton && ne.action == Event::Action::Press && !isMouseOver())
		{
			m_ExitFunc();
			return true;
		}

		return false;
	}

	default:
		return MenuObject::eventCallback(e);
	}
}

bool MenuBackground::isMouseOver()
{
	// Returns if teh mouse is over the background
	Vec2f mousePos = Event::getMousePos();
	return !(mousePos.x < x - width / 2 || mousePos.x > x + width / 2 || mousePos.y < y - height / 2 || mousePos.y > y + height / 2);
}
