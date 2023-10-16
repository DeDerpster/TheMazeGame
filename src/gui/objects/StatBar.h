#pragma once

#include "MenuObject.h"

class StatBar : public MenuObject
{
  private:
	const float *m_Stat, *m_StatMax;
	glm::vec4    m_BackgroundColour;

  public:
	StatBar(float x, float y, float width, float height, Layer *layer, const float *stat, const float *statMax);
	StatBar(std::function<void(float *, float *, float *, float *)> posFunc, Layer *layer, const float *stat, const float *statMax);
	StatBar(float x, float y, float width, float height, Layer *layer, const float *stat, const float *statMax, glm::vec4 backgroundColour);
	StatBar(std::function<void(float *, float *, float *, float *)> posFunc, Layer *layer, const float *stat, const float *statMax, glm::vec4 backgroundColour);
	virtual ~StatBar();

	virtual void update() override;
	virtual void render() override;
	virtual bool eventCallback(const Event::Event &e) override;

	void setStat(float *stat, float *statMax);
};