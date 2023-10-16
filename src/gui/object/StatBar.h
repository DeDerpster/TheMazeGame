#pragma once

#include "MenuObject.h"

#include "Button.h"

#include <string>

class StatBar : public MenuObject
{
  private:
	std::string   m_Name;    // Stores its name
	Button::State m_State;   // Stores the state
	// Stores the stat information
	const float *m_Stat, *m_StatMax;
	// Stores its colour
	glm::vec4    m_BackgroundColour;

  public:
	StatBar(std::string name, float x, float y, float width, float height, Layer *layer, const float *stat, const float *statMax);
	StatBar(std::string name, std::function<void(float *, float *, float *, float *)> posFunc, Layer *layer, const float *stat, const float *statMax);
	StatBar(std::string name, float x, float y, float width, float height, Layer *layer, const float *stat, const float *statMax, glm::vec4 backgroundColour);
	StatBar(std::string name, std::function<void(float *, float *, float *, float *)> posFunc, Layer *layer, const float *stat, const float *statMax, glm::vec4 backgroundColour);
	virtual ~StatBar() override;

	virtual void update() override;
	virtual void render() override;
	virtual bool eventCallback(const Event::Event &e) override;

	// Changes the stats
	void setStat(float *stat, float *statMax);
};