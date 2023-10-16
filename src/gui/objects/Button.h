#pragma once

#include "MenuObject.h"

#include <GLM.h>
#include <functional>
#include <string>

#include "Text.h"

class Button : public MenuObject
{
  public:
	enum State
	{
		None = 0,
		Hover,
		Press,
	};

  protected:
	Text m_Text;

	glm::vec4 m_BackgroundColour;
	glm::vec4 m_HoverColour;
	glm::vec4 m_PressColour;
	glm::vec4 m_BorderColour;

	uint16_t pressCalldown;

	State m_State;

	std::function<void()> buttonPressFunc;

  public:
	Button(Text text, float x, float y, float width, float height, Layer *layer, std::function<void()> buttonPressFunc);
	Button(Text text, std::function<void(float *, float *, float *, float *)> posFunc, Layer *layer, std::function<void()> buttonPressFunc);
	Button(Text text, float x, float y, float width, float height, Layer *layer, glm::vec4 backgroundColour, glm::vec4 hoverColour, glm::vec4 pressColour, std::function<void()> buttonPressFunc);
	Button(Text text, std::function<void(float *, float *, float *, float *)> posFunc, Layer *layer, glm::vec4 backgroundColour, glm::vec4 hoverColour, glm::vec4 pressColour, std::function<void()> buttonPressFunc);
	virtual ~Button() override;

	virtual void update() override;
	virtual void render() override;
	virtual bool eventCallback(const Event::Event &e) override;
};