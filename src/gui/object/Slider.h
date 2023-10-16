#pragma once

#include "MenuObject.h"

#include "Button.h"

class Slider : public MenuObject
{
  private:
	std::string   m_Name;    // Stores the name of the slider
	Button::State m_State;   // Stores the state of the button

	// Sotres the minimum and maximum of the slider
	float m_Min;
	float m_Max;

	// Stores the current position of the slider and how many stops there are
	uint32_t m_CurrPos;
	uint32_t m_Stops;

	// Stores the funcion that is called when the value has changed
	std::function<void(float)> m_Callback;

  public:
	Slider(std::string name, float x, float y, float width, float height, Layer *layer, float min, float max, float startVal, float step, std::function<void(float)> m_Callback);
	Slider(std::string name, std::function<void(float *, float *, float *, float *)> posFunc, Layer *layer, float min, float max, float startVal, float step, std::function<void(float)> m_Callback);
	virtual ~Slider() override;

	virtual void update() override;
	virtual void render() override;
	virtual bool eventCallback(const Event::Event &e) override;
};