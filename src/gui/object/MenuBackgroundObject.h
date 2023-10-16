#pragma once

#include "MenuObject.h"

#include <GLM.h>

class MenuBackground : public MenuObject
{
  private:
	glm::vec4             m_Colour;
	std::function<void()> m_ExitFunc;

  public:
	MenuBackground(float x, float y, float width, float height, Layer *layer, glm::vec4 colour, std::function<void()> exitFunc);
	MenuBackground(std::function<void(float *, float *, float *, float *)> posFunc, Layer *layer, glm::vec4 colour, std::function<void()> exitFunc);
	virtual ~MenuBackground() override;

	virtual void render() override;
	virtual void update() override;
	virtual bool eventCallback(const Event::Event &e) override;
};