#include "Slider.h"

#include "event/input/Mouse.h"
#include "rendering/Renderer.h"

#include "Application.h"

Slider::Slider(std::string name, float x, float y, float width, float height, Layer *layer, float min, float max, float startVal, float step, std::function<void(float)> callback)
	: MenuObject(x, y, width, height, layer),
	  m_Name(name),
	  m_State(Button::State::None),
	  m_Min(min),
	  m_Max(max),
	  m_CurrPos((uint32_t) round((startVal - min) / step)),
	  m_Stops((max - min) / step),
	  m_Callback(callback)
{
	if(max <= min)
		Log::error("Minimum is greater than the maximum when creating the slider!", LOGINFO);
}

Slider::Slider(std::string name, std::function<void(float *, float *, float *, float *)> posFunc, Layer *layer, float min, float max, float startVal, float step, std::function<void(float)> callback)
	: MenuObject(posFunc, layer),
	  m_Name(name),
	  m_State(Button::State::None),
	  m_Min(min),
	  m_Max(max),
	  m_CurrPos((uint32_t) round((startVal - min) / step)),
	  m_Stops((max - min) / step),
	  m_Callback(callback)
{
	if(max <= min)
		Log::error("Minimum is greater than the maximum when creating the slider!", LOGINFO);
}

Slider::~Slider()
{
}

void Slider::update()
{
	if(m_State != Button::State::Press)
	{
		// If the slider is not being pressed it will update the state
		Vec2f mousePos = Event::getMousePos();
		if(mousePos.x > x - width / 2 && mousePos.x < x + width / 2 && mousePos.y > y - height / 2 && mousePos.y < y + height / 2)
			m_State = Button::State::Hover;
		else
			m_State = Button::State::None;
	}
	else
	{
		// If the button is being pressed it will update its position relative to the mouse
		Vec2f mousePos    = Event::getMousePos();
		float relMousePos = mousePos.x - x + width / 2.0f;

		if(relMousePos < 0)
			m_CurrPos = 0;
		else if(relMousePos > width)
			m_CurrPos = m_Stops;
		else
			m_CurrPos = (uint32_t) round(((relMousePos / width) * ((float) m_Stops)));

		// NOTE: It does not call the callback function as it can cause issues so it will only be called when the user has released the button
	}
}

void Slider::render()
{
	// Renders the bar with half the height
	uint8_t layer = 7;
	Render::rectangle(x, y, 0.0f, width, height / 2.0f, {0.4f, 0.4f, 0.4f, 1.0f}, layer, true, true);

	// Renders the slider at the correct position
	float per  = (float) m_CurrPos / ((float) m_Stops);
	float barX = width * per + x - width / 2;
	Render::rectangle(barX, y, 0.0f, 10.0f * Application::getGUIScale(), height, {0.2f, 0.2f, 0.2f, 1.0f}, layer + 1, true, true);

	if(m_State == Button::State::Hover)
	{
		// NOTE: This will not be shown when the slider is being pressed
		// If the mouse is hovering over it it will render the name
		float scale    = 35.0f * Application::getGUIScale();
		Vec2f mousePos = Event::getMousePos();

		Render::hoverText(m_Name, mousePos.x, mousePos.y, scale, {1.0f, 1.0f, 1.0f, 1.0f}, {0.3f, 0.3f, 0.3f, 0.7f}, layer + 1, true);
	}
}

bool Slider::eventCallback(const Event::Event &e)
{
	switch(e.getType())
	{
	case Event::EventType::MouseClicked:
	{
		const Event::MouseClickedEvent &ne = static_cast<const Event::MouseClickedEvent &>(e);

		switch(ne.action)
		{
		case Event::Action::Press:
			// If the mouse is over the slider it will change its state
			if(m_State == Button::State::Hover)
			{
				m_State = Button::State::Press;
				return true;
			}
			break;

		case Event::Action::Release:
			// If the mouse button is released and its currently being pressed it will update the value by calling the callback
			// NOTE: The mouse does not have to be over the slider for this to happen
			if(m_State == Button::State::Press)
			{
				m_State = Button::State::None;

				float per = (float) m_CurrPos / ((float) m_Stops);
				float num = m_Min + per * (m_Max - m_Min);
				m_Callback(num);

				return true;
			}
			break;

		case Event::Action::Repeat:
			// If the slider is being pressed it will return true (Just so it doesn't mess other things up)
			if(m_State == Button::State::Press)
				return true;
			break;

		default:
			break;
		}

		return false;
	}

	default:
		return MenuObject::eventCallback(e);
	}
}