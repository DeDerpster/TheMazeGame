#include "MenuBackgroundObject.h"

#include "Renderer.h"

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
	Render::rectangle(x, y, 0.0f, width, height, m_Colour, true, true, true);
}

void MenuBackground::update()
{
}

bool MenuBackground::eventCallback(const Event::Event &e)
{
	// TODO: Add exit func
	if(e.getType() == Event::EventType::keyInput)
	{
		const Event::KeyboardEvent &ne = static_cast<const Event::KeyboardEvent &>(e);
		if(ne.key == GLFW_KEY_ESCAPE && ne.action == GLFW_PRESS)
			m_ExitFunc();
	}
	else if(e.getType() == Event::EventType::mouseClicked)
	{
		const Event::MouseClickedEvent &ne = static_cast<const Event::MouseClickedEvent &>(e);
		if(ne.button == GLFW_MOUSE_BUTTON_LEFT && (ne.pos.x < x - width / 2 || ne.pos.x > x + width / 2 || ne.pos.y < y - height / 2 || ne.pos.y > y + height / 2))
			m_ExitFunc();
	}
	return MenuObject::eventCallback(e);
}
