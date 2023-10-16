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
	uint8_t layer = 7;
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

		if(ne.button == Event::MouseButton::leftButton && ne.action == Event::Action::Press && (ne.pos.x < x - width / 2 || ne.pos.x > x + width / 2 || ne.pos.y < y - height / 2 || ne.pos.y > y + height / 2))
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
